#include "TileModel.h"

namespace serio::qt {

TileModel::TileModel(QString title, QString subtitle)
    : title(std::move(title)), subtitle(std::move(subtitle)) {}

void TileModel::setTitle(QString title) {
    this->title = std::move(title);
    emit titleChanged();
}

void TileModel::setSubtitle(QString subtitle) {
    this->subtitle = std::move(subtitle);
    emit subtitleChanged();
}

QString TileModel::getTitle() const {
    return title;
}

QString TileModel::getSubtitle() const {
    return subtitle;
}

QString TileModel::getIcon() const {
    return QString();
}

}