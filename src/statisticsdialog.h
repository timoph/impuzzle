#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticsDialog(QWidget *parent = 0);

private:
    QLabel *label_;
    QVBoxLayout *mainLayout_;
};

#endif
