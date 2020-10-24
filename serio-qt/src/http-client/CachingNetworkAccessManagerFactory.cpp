#include <utility>
#include <QNetworkDiskCache>
#include "CachingNetworkAccessManagerFactory.h"

namespace serio::qt {

CachingNetworkAccessManagerFactory::CachingNetworkAccessManagerFactory(QString diskCacheDirectory)
    : diskCacheDirectory(std::move(diskCacheDirectory)) {}

QNetworkAccessManager *CachingNetworkAccessManagerFactory::create(QObject *parent) {
    auto manager = new QNetworkAccessManager(parent);
    auto cache = new QNetworkDiskCache(parent);
    cache->setCacheDirectory(diskCacheDirectory);
    manager->setCache(cache);
    return manager;
}

}