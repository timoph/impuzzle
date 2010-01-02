#include "settingsdialog.h"
#include "settings.h"
#include "defines.h"

#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

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

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(buttonGroup_);

    setLayout(mainLayout_);

    connect(easyButton_, SIGNAL(toggled(bool)), this, SLOT(difficultySelectionChanged(bool)));
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
