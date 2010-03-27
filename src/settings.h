#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPixmap>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings *instance();

    int pieceCount() const;
    void setPieceCount(const int pieces);

    QPixmap image() const;
    void setImage(const QPixmap &image);

    QString imagePath() const;
    void setImagePath(const QString &path);

private:
    Settings(QObject *parent = 0);

    static Settings *instance_;

    int pieceCount_;
    QPixmap image_;
    QString imagePath_;
};
#endif
