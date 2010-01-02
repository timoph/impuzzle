#include "settings.h"

Settings *Settings::instance_ = 0;

Settings::Settings(QObject *parent) :
        QObject(parent)
{
    pieceCount_ = 12;
    image_ = 0;
}

Settings *Settings::instance()
{
    if(!instance_) {
        instance_ = new Settings;
    }

    return instance_;
}

int Settings::pieceCount() const
{
    return pieceCount_;
}

void Settings::setPieceCount(const int pieces)
{
    pieceCount_ = pieces;
}

QPixmap Settings::image() const
{
    return image_;
}

void Settings::setImage(const QPixmap &image)
{
    image_ = image;
}
