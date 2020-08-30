#include "FetchRequest.h"
#include <QNetworkReply>

serio::qt::FetchRequest::FetchRequest(std::vector<std::string> linksToFetch, QThread* managerThread)
    : QObject(), linksToFetch(std::move(linksToFetch)), hasAlreadyFailed(false) {
    moveToThread(managerThread);
}

void serio::qt::FetchRequest::execute(QNetworkAccessManager& manager) {
    finishIfNothingToDo();
    sendRequestsToSpecifiedLinks(manager);
    writeRepliesIntoPromiseWhenFinished();
}

void serio::qt::FetchRequest::sendRequestsToSpecifiedLinks(QNetworkAccessManager& manager) {
    for (const std::string& link: linksToFetch) {
        QUrl url(QString::fromStdString(link));
        QNetworkRequest request(url);
        QNetworkReply* reply = manager.get(request);
        repliesInProgress.insert(reply);
    }
}

void serio::qt::FetchRequest::writeRepliesIntoPromiseWhenFinished() {
    rawResponses.reserve(repliesInProgress.size());
    for (QNetworkReply* reply: repliesInProgress) {
        connect(reply, &QNetworkReply::finished, this, [reply, this] () {handleFinishedReply(reply);});
    }
}

void serio::qt::FetchRequest::handleFinishedReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        readSuccessReply(reply);
    } else if (!hasAlreadyFailed) {
        readFailReply(reply);
    }
    disposeReply(reply);
    finishIfDone();
}

void serio::qt::FetchRequest::readSuccessReply(QNetworkReply *reply) {
    std::string rawResponse = reply->readAll().toStdString();
    rawResponses.push_back(std::move(rawResponse));
}

void serio::qt::FetchRequest::readFailReply(QNetworkReply *reply) {
    hasAlreadyFailed = true;
    writeReplyErrorIntoPromise(reply);
    abortRepliesInProgress();
}

void serio::qt::FetchRequest::disposeReply(QNetworkReply *reply) {
    repliesInProgress.erase(reply);
    reply->close();
    reply->deleteLater();
}

void serio::qt::FetchRequest::finishIfDone() {
    if (repliesInProgress.empty()) {
        finish();
    }
}

void serio::qt::FetchRequest::finishIfNothingToDo() {
    if (linksToFetch.empty()) {
        finish();
    }
}

void serio::qt::FetchRequest::finish() {
    if (!hasAlreadyFailed) {
        responsesPromise.set_value(std::move(rawResponses));
    }
    deleteLater();
}

void serio::qt::FetchRequest::abortRepliesInProgress() {
    std::set<QNetworkReply*> repliesToAbort = repliesInProgress;
    for (QNetworkReply* replyToAbort: repliesToAbort) {
        replyToAbort->abort();
    }
}

void serio::qt::FetchRequest::writeReplyErrorIntoPromise(QNetworkReply *reply) {
    std::string failedUrl = reply->url().toString().toStdString();
    std::string errorMessage = reply->errorString().toStdString();
    FetchRequestError error(failedUrl, errorMessage);
    responsesPromise.set_exception(std::make_exception_ptr(error));
}

std::future<std::vector<std::string>> serio::qt::FetchRequest::getFutureOfResponses() {
    return responsesPromise.get_future();
}

serio::qt::FetchRequestError::FetchRequestError(const std::string& failedUrl, const std::string& cause)
    : runtime_error("Failed to fetch content of '" + failedUrl + "': Reason: " + cause) {}
