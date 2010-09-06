#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QList>

class Statistics : public QObject
{
    Q_OBJECT

public:
    enum Difficulty {
        easyDifficulty = 0,
        hardDifficulty
    };

    static Statistics *instance();

    int gameCount(Difficulty difficulty) const;

    int totalGameCount() const;

    qreal averageMoves(Difficulty difficulty) const;

    int minMoves(Difficulty difficulty) const;

    int maxMoves(Difficulty difficulty) const;

public slots:
    void addNewScore(int moves, Difficulty difficulty);

    void increaseGameCount(Difficulty difficulty);

    void readFile();

    void saveFile();

private:
    Statistics(QObject *parent = 0);

    static Statistics *instance_;

    QList<int> moves_;
    QList<int> minMoves_;
    QList<int> maxMoves_;
    QList<int> games_;
};

#endif
