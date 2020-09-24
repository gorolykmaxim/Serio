#include "TileModel.h"

serio::qt::TileModel::TileModel(QString title, QString subtitle)
    : title(std::move(title)), subtitle(std::move(subtitle)) {}

void serio::qt::TileModel::setTitle(QString title) {
    this->title = std::move(title);
    emit titleChanged();
}

void serio::qt::TileModel::setSubtitle(QString subtitle) {
    this->subtitle = std::move(subtitle);
    emit subtitleChanged();
}

QString serio::qt::TileModel::getTitle() const {
    return title;
}

QString serio::qt::TileModel::getSubtitle() const {
    return subtitle;
}

QString serio::qt::TileModel::getIcon() const {
    return QString();
}
