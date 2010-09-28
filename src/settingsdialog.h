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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QVBoxLayout;
class QGroupBox;
class QRadioButton;
class QHBoxLayout;
class QComboBox;
class QLabel;
class QPushButton;
class TrackerFiles;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0);

public slots:
    int exec();

private slots:
    void difficultySelectionChanged(bool value);
    void imageSelectionChanged(const QString &txt);
    void applyClicked();
    void trackerFilesRead(const QStringList &files);

private:
    QVBoxLayout *mainLayout_;
    QHBoxLayout *buttonLayout_;

    QRadioButton *easyButton_;
    QRadioButton *hardButton_;

    QGroupBox *buttonGroup_;

    QComboBox *imageCombo_;

    QLabel *selectedImageLabel_;

    QPushButton *applyButton_;

    TrackerFiles *trackerFiles_;
};
#endif
