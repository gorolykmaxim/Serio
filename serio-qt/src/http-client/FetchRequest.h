#ifndef SERIO_FETCHREQUEST_H
#define SERIO_FETCHREQUEST_H

#include <future>
#include <set>
#include <QObject>
#include <QNetworkAccessManager>

namespace serio::qt {

/**
 * Instances of this class should always be created on the heap using raw pointer: the instance handles it's lifecycle
 * on its own and will delete itself when it is appropriate.
 */
class FetchRequest : public QObject {
    Q_OBJECT
public:
    FetchRequest(std::vector<std::string> linksToFetch, QThread* managerThread);
    void execute(QNetworkAccessManager& manager);
    std::future<std::vector<std::string>> getFutureOfResponses();
private:
    bool hasAlreadyFailed;
    const std::vector<std::string> linksToFetch;
    std::set<QNetworkReply*> repliesInProgress;
    std::vector<std::string> rawResponses;
    std::promise<std::vector<std::string>> responsesPromise;
    void sendRequestsToSpecifiedLinks(QNetworkAccessManager& manager);
    void writeRepliesIntoPromiseWhenFinished();
    void handleFinishedReply(QNetworkReply* reply);
    void readSuccessReply(QNetworkReply* reply);
    void readFailReply(QNetworkReply* reply);
    void disposeReply(QNetworkReply* reply);
    void finishIfNothingToDo();
    void finishIfDone();
    void finish();
    void abortRepliesInProgress();
    void writeReplyErrorIntoPromise(QNetworkReply* reply);
};

class FetchRequestError : public std::runtime_error {
public:
    FetchRequestError(const std::string& failedUrl, const std::string& cause);
};

}

#endif //SERIO_FETCHREQUEST_H
