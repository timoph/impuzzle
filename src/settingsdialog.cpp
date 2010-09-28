/*
  Image Puzzle - A set your pieces straight game
  Copyright (C) 2009  Timo Härkönen

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settingsdialog.h"
#include "settings.h"
#include "defines.h"
#include "trackerfiles.h"

#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QDir>
#include <QLabel>

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent)
{
    setModal(true);

    easyButton_ = new QRadioButton(tr("Easy"));
    easyButton_->setChecked(true);
    hardButton_ = new QRadioButton(tr("Hard"));

    buttonLayout_ = new QHBoxLayout;
    buttonLayout_->addWidget(easyButton_);
    buttonLayout_->addWidget(hardButton_);

    buttonGroup_ = new QGroupBox(tr("Difficulty"));
    buttonGroup_->setLayout(buttonLayout_);

    QStringList items;
    items << DEFAULT_IMAGE_TXT << RANDOM_IMAGE_TXT << SELECT_IMAGE_TXT;

    imageCombo_ = new QComboBox;
    imageCombo_->addItems(items);
    imageCombo_->setCurrentIndex(1);

    selectedImageLabel_ = new QLabel(tr("n/a"));

    applyButton_ = new QPushButton(tr("Play"));

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(buttonGroup_);
    mainLayout_->addWidget(imageCombo_);
    mainLayout_->addWidget(selectedImageLabel_);
    mainLayout_->addWidget(applyButton_);

    selectedImageLabel_->setVisible(false);

    trackerFiles_ = new TrackerFiles(this);

    setLayout(mainLayout_);

    connect(easyButton_, SIGNAL(toggled(bool)), this, SLOT(difficultySelectionChanged(bool)));
    //connect(imageCombo_, SIGNAL(currentIndexChanged(QString)), this, SLOT(imageSelectionChanged(QString)));
    connect(imageCombo_, SIGNAL(activated(QString)), this, SLOT(imageSelectionChanged(QString)));
    connect(applyButton_, SIGNAL(clicked()), this, SLOT(applyClicked()));
    connect(trackerFiles_, SIGNAL(filesRead(QStringList)), this, SLOT(trackerFilesRead(QStringList)));
}

int SettingsDialog::exec()
{
    // Making sure that a random image is picked when starting a new game after the first game (fix me)
    if(imageCombo_->currentText() == RANDOM_IMAGE_TXT) {
        imageSelectionChanged(RANDOM_IMAGE_TXT);
    }

    return QDialog::exec();
}

void SettingsDialog::difficultySelectionChanged(bool value)
{
    if(value) {
        Settings::instance()->setPieceCount(EASY_PIECE_COUNT);
    }
    else {
        Settings::instance()->setPieceCount(HARD_PIECE_COUNT);
    }
}

void SettingsDialog::imageSelectionChanged(const QString &txt)
{
    if(txt == RANDOM_IMAGE_TXT) {
        if(Settings::instance()->localImages().isEmpty()) {
            trackerFiles_->readFiles();
        }
        else {
            trackerFilesRead(Settings::instance()->localImages());
        }
    }
    else if(txt == SELECT_IMAGE_TXT) {
        //qDebug() << "Select image... selected";

        // Open file selection dialog
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::homePath() + QLatin1String("/MyDocs/.images"),
                                                         tr("Images (*.png *.xpm *.jpg)"));

        Settings::instance()->setImage(QPixmap(fileName));
        Settings::instance()->setImagePath(fileName);

        selectedImageLabel_->setText(fileName);

        if(!selectedImageLabel_->isVisible() && !fileName.isEmpty()) {
            selectedImageLabel_->setVisible(true);
        }
    }
    else {
        //qDebug() << "Default image selected";

        Settings::instance()->setImage(0);
        Settings::instance()->setImagePath("default");

        if(selectedImageLabel_->isVisible()) {
            selectedImageLabel_->setVisible(false);
        }
    }
}

void SettingsDialog::applyClicked()
{
    this->done(1);
}

void SettingsDialog::trackerFilesRead(const QStringList &files)
{
    if(!files.isEmpty()) {
        QString path = files.at(qrand() % files.count());
        path = path.trimmed();
        Settings::instance()->setImage(QPixmap(path));
        Settings::instance()->setImagePath(path);

        if(Settings::instance()->localImages().isEmpty()) {
            Settings::instance()->setLocalImages(files);
        }
    }
    else {
        Settings::instance()->setImage(QPixmap(":/images/default.jpg"));
        Settings::instance()->setImagePath("default");
    }

    if(selectedImageLabel_->isVisible()) {
        selectedImageLabel_->setVisible(false);
    }
}
