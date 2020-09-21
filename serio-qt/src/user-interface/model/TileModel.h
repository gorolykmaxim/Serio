#ifndef SERIO_TILEMODEL_H
#define SERIO_TILEMODEL_H

#include <QObject>

namespace serio::qt {

class TileModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ getSubtitle WRITE setSubtitle NOTIFY subtitleChanged)
public:
    explicit TileModel(QString title, QString subtitle = "");
    void setTitle(QString title);
    void setSubtitle(QString subtitle);
    [[nodiscard]] QString getTitle() const;
    [[nodiscard]] QString getSubtitle() const;
signals:
    void titleChanged();
    void subtitleChanged();
private:
    QString title;
    QString subtitle;
};

}

#endif //SERIO_TILEMODEL_H
