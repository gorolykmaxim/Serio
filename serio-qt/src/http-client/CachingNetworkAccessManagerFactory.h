#ifndef SERIO_CACHINGNETWORKACCESSMANAGERFACTORY_H
#define SERIO_CACHINGNETWORKACCESSMANAGERFACTORY_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

namespace serio::qt {

class CachingNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory {
public:
    explicit CachingNetworkAccessManagerFactory(QString diskCacheDirectory);
    QNetworkAccessManager *create(QObject *parent) override;
private:
    QString diskCacheDirectory;
};

}

#endif //SERIO_CACHINGNETWORKACCESSMANAGERFACTORY_H
