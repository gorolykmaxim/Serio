#include "FetchRequest.h"
#include <QNetworkReply>

namespace serio::qt {

FetchRequest::FetchRequest(std::vector<std::string> linksToFetch, QThread* managerThread)
    : QObject(), linksToFetch(std::move(linksToFetch)), hasAlreadyFailed(false) {
    moveToThread(managerThread);
}

void FetchRequest::execute(QNetworkAccessManager& manager) {
    finishIfNothingToDo();
    sendRequestsToSpecifiedLinks(manager);
    writeRepliesIntoPromiseWhenFinished();
}

void FetchRequest::sendRequestsToSpecifiedLinks(QNetworkAccessManager& manager) {
    for (const auto& link: linksToFetch) {
        QUrl url(QString::fromStdString(link));
        QNetworkRequest request(url);
        auto reply = manager.get(request);
        repliesInProgress.insert(reply);
    }
}

void FetchRequest::writeRepliesIntoPromiseWhenFinished() {
    rawResponses.reserve(repliesInProgress.size());
    for (auto reply: repliesInProgress) {
        connect(reply, &QNetworkReply::finished, this, [reply, this] () {handleFinishedReply(reply);});
    }
}

void FetchRequest::handleFinishedReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        readSuccessReply(reply);
    } else if (!hasAlreadyFailed) {
        readFailReply(reply);
    }
    disposeReply(reply);
    finishIfDone();
}

void FetchRequest::readSuccessReply(QNetworkReply *reply) {
    auto rawResponse = reply->readAll().toStdString();
    rawResponses.push_back(std::move(rawResponse));
}

void FetchRequest::readFailReply(QNetworkReply *reply) {
    hasAlreadyFailed = true;
    writeReplyErrorIntoPromise(reply);
    abortRepliesInProgress();
}

void FetchRequest::disposeReply(QNetworkReply *reply) {
    repliesInProgress.erase(reply);
    reply->close();
    reply->deleteLater();
}

void FetchRequest::finishIfDone() {
    if (repliesInProgress.empty()) {
        finish();
    }
}

void FetchRequest::finishIfNothingToDo() {
    if (linksToFetch.empty()) {
        finish();
    }
}

void FetchRequest::finish() {
    if (!hasAlreadyFailed) {
        responsesPromise.set_value(std::move(rawResponses));
    }
    deleteLater();
}

void FetchRequest::abortRepliesInProgress() {
    auto repliesToAbort = repliesInProgress;
    for (auto replyToAbort: repliesToAbort) {
        replyToAbort->abort();
    }
}

void FetchRequest::writeReplyErrorIntoPromise(QNetworkReply *reply) {
    auto failedUrl = reply->url().toString().toStdString();
    auto errorMessage = reply->errorString().toStdString();
    FetchRequestError error(failedUrl, errorMessage);
    responsesPromise.set_exception(std::make_exception_ptr(error));
}

std::future<std::vector<std::string>> FetchRequest::getFutureOfResponses() {
    return responsesPromise.get_future();
}

FetchRequestError::FetchRequestError(const std::string& failedUrl, const std::string& cause)
    : runtime_error("Failed to fetch content of '" + failedUrl + "': Reason: " + cause) {}

}