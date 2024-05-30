#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboardview.h"
#include "ciberopponent.h"
#include <QLabel>

const QString Pcircle = QObject::tr("Circle");
const QString Pcross = QObject::tr("Cross");

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    chessBoardView* m_chessboard;
    CiberOpponent* m_opponent;
    bool m_circleTurn;
    bool m_PlayerisCircle;

    QLabel* m_lblPlayer;

    bool m_single;

private slots:
    void ManulChess(int x, int y);
    void OpponentFinished();
    void GameOver(int i);

private:
    void ResetGame();
};

#endif // MAINWINDOW_H
