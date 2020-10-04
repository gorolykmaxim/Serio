#ifndef SERIO_QHTTPCLIENT_H
#define SERIO_QHTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <http-client/HttpClient.h>
#include <QQmlApplicationEngine>
#include "FetchRequest.h"
#include "CachingNetworkAccessManagerFactory.h"

Q_DECLARE_METATYPE(serio::qt::FetchRequest*);

namespace serio::qt {

/**
 * Executes HTTP requests on the thread, on which it is created. Such thread should have QT event loop running on it.
 *
 * This class is thread-safe: it's methods can be called from multiple threads, while the actual requests will be sent
 * from the client's parent thread.
 */
class QHttpClient : public QObject, public core::HttpClient {
    Q_OBJECT
public:
    explicit QHttpClient(int timeout = 5000, QNetworkRequest::RedirectPolicy redirectPolicy = QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy);
    void assignTo(QQmlApplicationEngine& engine);
    std::future<std::vector<std::string>> fetchContentFromLinks(const std::vector<std::string> &links) override;
signals:
    void requestLinksContentFetch(FetchRequest* request);
private slots:
    void fetchContentFromLinksIntoPromise(FetchRequest* request);
private:
    CachingNetworkAccessManagerFactory managerFactory;
    QNetworkAccessManager manager;
};

}

#endif //SERIO_QHTTPCLIENT_H
