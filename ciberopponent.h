#ifndef CIBEROPPONENT_H
#define CIBEROPPONENT_H
#include <QObject>
#include "chessboardview.h"

class CiberOpponent: public QObject
{
    Q_OBJECT
public:
    CiberOpponent(chessBoardView* chessboard);

    void SetEnable(int playerID);
    void DoChess();

    bool IsEnable() {return m_enable;}

private:
    bool m_enable;
    bool m_isCircle;
    chessBoardView* m_chessboard;

signals:
    void WaitingForPlayer();

private slots:
    void GameOver(int i);
};

#endif // CIBEROPPONENT_H
