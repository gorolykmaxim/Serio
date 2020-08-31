#include "QHttpClient.h"

serio::qt::QHttpClient::QHttpClient(int timeout, QNetworkRequest::RedirectPolicy redirectPolicy) : QObject() {
    manager.setTransferTimeout(timeout);
    manager.setRedirectPolicy(redirectPolicy);
    connect(this, &QHttpClient::requestLinksContentFetch, this, &QHttpClient::fetchContentFromLinksIntoPromise, Qt::QueuedConnection);
}

std::future<std::vector<std::string>> serio::qt::QHttpClient::fetchContentFromLinks(const std::vector<std::string> &links) {
    auto request = new FetchRequest(links, manager.thread());
    emit requestLinksContentFetch(request);
    return request->getFutureOfResponses();
}

void serio::qt::QHttpClient::fetchContentFromLinksIntoPromise(FetchRequest* request) {
    request->execute(manager);
}