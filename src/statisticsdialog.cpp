#include "statisticsdialog.h"
#include "statistics.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

StatisticsDialog::StatisticsDialog(QWidget *parent) :
        QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Statistics"));

    resetButton_ = new QPushButton(tr("Reset"));
    doneButton_ = new QPushButton(tr("Close"));

    buttonLayout_ = new QHBoxLayout;
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(resetButton_);
    buttonLayout_->addWidget(doneButton_);

    label_ = new QLabel;

    updateContent();

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(label_);
    mainLayout_->addLayout(buttonLayout_);

    setLayout(mainLayout_);

    connect(resetButton_, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(doneButton_, SIGNAL(clicked()), this, SLOT(close()));
}

void StatisticsDialog::updateContent()
{
    QString txt = QString("\tEasy\tHard\nGames\t%1\t%5\nBest\t%2\t%6\nAvg\t%3\t%7\nWorst\t%4\t%8\n")
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::hardDifficulty)));

    label_->setText(txt);
}

void StatisticsDialog::resetClicked()
{
    Statistics::instance()->resetStatistics();
    updateContent();
}
