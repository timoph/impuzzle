#include "statisticsdialog.h"
#include "statistics.h"

#include <QFormLayout>
#include <QLabel>

StatisticsDialog::StatisticsDialog(QWidget *parent) :
        QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Statistics"));

    label_ = new QLabel;

    QString txt = QString("Easy:\nGames\t%1\nBest\t%2\nAvg\t%3\nWorst\t%4\n\nHard:\nGames\t%5\nBest\t%6\nAvg\t%7\nWorst\t%8")
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::hardDifficulty)));

    label_->setText(txt);

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(label_);

    setLayout(mainLayout_);
}
