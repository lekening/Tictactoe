#include "mainwindow.h"
#include <QMessageBox>
#include <QBoxLayout>
#include<QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_chessboard = new chessBoardView(this);
    m_opponent = new CiberOpponent(m_chessboard);

    connect(m_chessboard, &chessBoardView::NewChessPosition, this, &MainWindow::ManulChess);
    connect(m_chessboard, &chessBoardView::HaveWinner, this, &MainWindow::GameOver);
    connect(m_opponent, &CiberOpponent::WaitingForPlayer, this, &MainWindow::OpponentFinished);

    m_lblPlayer = new QLabel(Pcircle, this);

    QHBoxLayout* hlay = new QHBoxLayout;
    hlay->addStretch(0);
    hlay->addWidget(m_lblPlayer);
    hlay->addWidget(new QLabel(tr("'s Turn"), this));

    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addLayout(hlay);
    vlay->addWidget(m_chessboard);
    QWidget* w = new QWidget;
    w->setLayout(vlay);
    this->setCentralWidget(w);

    ResetGame();
}

MainWindow::~MainWindow()
{
    delete m_opponent;
}

void MainWindow::ManulChess(int x, int y)
{
    if(m_single&&m_PlayerisCircle != m_circleTurn) {
        m_PlayerisCircle = m_circleTurn;
        return;
    }
    if(m_chessboard->addChess(x, y, m_circleTurn)) {
        m_circleTurn = !m_circleTurn;

        m_lblPlayer->setText(m_circleTurn ? Pcircle : Pcross);

        if(m_single) { m_opponent->DoChess(); }
    }


}

void MainWindow::OpponentFinished()
{
    //if(m_PlayerisCircle == m_circleTurn) { return; }
    m_circleTurn = !m_circleTurn;
    m_lblPlayer->setText(m_circleTurn ? Pcircle : Pcross);
}

void MainWindow::GameOver(int i)
{
    QString winner;
    if(i == 0) { winner = tr("None"); }
    else if(i == 1) { winner = Pcircle; }
    else if(i == 2) { winner = Pcross; }

    QString str = tr("Winner:");

    if(QMessageBox::information(this, tr("Finish"), str + winner, QMessageBox::Ok) == QMessageBox::Ok) {
        ResetGame();
    }
}

void MainWindow::ResetGame()
{
    m_chessboard->reset();
    m_circleTurn = true;
    m_lblPlayer->setText(m_circleTurn ? Pcircle : Pcross);
    if(QMessageBox::question(this, tr("Single or Double"), tr("Play With Computer?"), QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok) {
        m_single = true;
        m_opponent->SetEnable(2);
    } else {
        m_single = false;
        m_opponent->SetEnable(0);
    }
    m_PlayerisCircle = true;
}
