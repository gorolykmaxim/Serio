#include <utility>
#include <QNetworkDiskCache>
#include "CachingNetworkAccessManagerFactory.h"

serio::qt::CachingNetworkAccessManagerFactory::CachingNetworkAccessManagerFactory(QString diskCacheDirectory)
    : diskCacheDirectory(std::move(diskCacheDirectory)) {}

QNetworkAccessManager *serio::qt::CachingNetworkAccessManagerFactory::create(QObject *parent) {
    auto manager = new QNetworkAccessManager(parent);
    auto cache = new QNetworkDiskCache(parent);
    cache->setCacheDirectory(diskCacheDirectory);
    manager->setCache(cache);
    return manager;
}
