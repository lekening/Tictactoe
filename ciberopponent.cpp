#include "ciberopponent.h"
#include <QtDebug>

CiberOpponent::CiberOpponent(chessBoardView *chessboard)
    : QObject(nullptr)
{
    m_enable = false;
    m_chessboard = chessboard;

    connect(m_chessboard, &chessBoardView::HaveWinner, this, &CiberOpponent::GameOver);
}

void CiberOpponent::SetEnable(int playerID)
{
    m_isCircle = playerID == 1;
    m_enable = playerID > 0;
}

void CiberOpponent::DoChess()
{
    if(!m_enable) { return; }

    int best_x = 1;
    int best_y = 1;
    int best_s = 1;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int t_s = m_chessboard->testChessPos(i, j, m_isCircle);
            if(t_s > best_s) {
                best_s = t_s;
                best_x = i;
                best_y = j;
            }
            if(best_s == 3) { break; }
        }
        if(best_s == 3) { break; }
    }

    if(m_chessboard->addChess(best_x, best_y, m_isCircle)) { emit WaitingForPlayer(); }
}

void CiberOpponent::GameOver(int i)
{
    m_enable = false;

}
