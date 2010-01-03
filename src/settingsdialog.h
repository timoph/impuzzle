#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QVBoxLayout;
class QGroupBox;
class QRadioButton;
class QHBoxLayout;
class QComboBox;
class QLabel;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0);

private slots:
    void difficultySelectionChanged(bool value);
    void imageSelectionChanged(const QString &txt);

private:
    QVBoxLayout *mainLayout_;
    QHBoxLayout *buttonLayout_;

    QRadioButton *easyButton_;
    QRadioButton *hardButton_;

    QGroupBox *buttonGroup_;

    QComboBox *imageCombo_;

    QLabel *selectedImageLabel_;
};
#endif
