#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticsDialog(QWidget *parent = 0);

private slots:
    void updateContent();
    void resetClicked();

private:
    QLabel *label_;
    QPushButton *resetButton_;
    QPushButton *doneButton_;
    QVBoxLayout *mainLayout_;
    QHBoxLayout *buttonLayout_;
};

#endif
