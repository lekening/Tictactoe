#include "chessboardview.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QOpenGLFunctions>
#include <QtMath>
#include <QtDebug>

chessBoardView::chessBoardView(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(300, 300);
    reset();
}

bool chessBoardView::addChess(int x, int y, bool circle)
{
    if(x < 0) { return false; }
    if(x > 2) { return false; }
    if(y < 0) { return false; }
    if(y > 2) { return false; }

    if(m_chess[y][x] > 0) { return false; }

    m_chess[y][x] = circle ? 1 : 2;
    qDebug() << "~~~~~~~~~~~~~~~";
    qDebug() << m_chess[0][0] << m_chess[0][1] << m_chess[0][2];
    qDebug() << m_chess[1][0] << m_chess[1][1] << m_chess[1][2];
    qDebug() << m_chess[2][0] << m_chess[2][1] << m_chess[2][2];
    qDebug() << "~~~~~~~~~~~~~~~";
    update();

    if(JudgeWinner(x, y)) { emit HaveWinner(circle ? 1 : 2); return false;}
    if(CheckDraw()) { emit HaveWinner(0); return false;}

    return true;
}

int chessBoardView::testChessPos(int x, int y, bool circle)
{
    if(x < 0) { return 0; }
    if(x > 2) { return 0; }
    if(y < 0) { return 0; }
    if(y > 2) { return 0; }

    if(m_chess[y][x] > 0) { return 0; }

    m_chess[y][x] = circle ? 1 : 2;
    if(JudgeWinner(x, y)) {
        m_chess[y][x] = 0;
        return 3;
    }

    m_chess[y][x] = circle ? 2 : 1;
    if(JudgeWinner(x, y)) {
        m_chess[y][x] = 0;
        return 2;
    }
    m_chess[y][x] = 0;
    return 1;
}

void chessBoardView::reset()
{
    memset(m_chess, 0, sizeof(m_chess));
    update();
}

void chessBoardView::initializeGL()
{
    //QOpenGLFunctions::initializeOpenGLFunctions();
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

}

void chessBoardView::resizeGL(int w, int h)
{
    if(w == 0) { w = 1; }
    if ( h == 0 ) { h = 1; }


    glViewport( 0, 0, w, h );
}

void chessBoardView::paintGL()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    int winWidth = this->width();
    int winHeight = this->height();

    int gridWidth = winWidth / 3;
    int gridHeight = winHeight / 3;

    glColor4f(0.0, 0.0, 0.0, 1.0);

    glBegin(GL_LINES);
    glLineWidth(10);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(3);
    glVertex3f(0.33, 1.0, 0.0);
    glVertex3f(0.33, -1.0, 0.0);
    glVertex3f(-0.33, 1.0, 0.0);
    glVertex3f(-0.33, -1.0, 0.0);
    glVertex3f(1.0, 0.33, 0.0);
    glVertex3f(-1.0, 0.33, 0.0);
    glVertex3f(-1.0, -0.33, 0.0);
    glVertex3f(1.0, -0.33, 0.0);
    glEnd();

    int radius = qMin(gridWidth, gridHeight) / 4;
    glLineWidth(4.0);
    for(int i = 0; i < 3; i++) {
        int xpos = gridWidth*i + gridWidth / 2;
        for(int j = 0; j < 3; j++) {
            int ypos = gridHeight*j + gridHeight / 2;
            switch (m_chess[j][i]) {
            case 1:
                DrawCircle(xpos, ypos, radius);
                break;
            case 2:
                DrawCross(xpos, ypos, radius);
                break;
            default:
                break;
            }
        }
    }


}

void chessBoardView::mousePressEvent(QMouseEvent *mEvent)
{
    QOpenGLWidget::mousePressEvent(mEvent);
}

void chessBoardView::mouseReleaseEvent(QMouseEvent *mEvent)
{
    QOpenGLWidget::mouseReleaseEvent(mEvent);
    QPoint pt = mEvent->pos();

    int winWidth = this->width();
    int winHeight = this->height();

    int gridWidth = winWidth / 3;
    int gridHeight = winHeight / 3;

    int x = ((int)pt.x()) / gridWidth;
    int y = ((int)pt.y()) / gridHeight;

    if(x < 0) { return; }
    if(x > 2) { return; }
    if(y < 0) { return; }
    if(y > 2) { return; }

    emit NewChessPosition(x, y);
}

void chessBoardView::DrawCross(int x, int y, int h)
{
    int winWidth = this->width();
    int winHeight = this->height();

    glBegin(GL_LINES);

    glVertex3f((x + h) * 2.0 / winWidth - 1.0, 1.0 - (y + h) * 2.0 / winHeight, 0.0);
    glVertex3f((x - h) * 2.0 / winWidth - 1.0, 1.0 - (y - h) * 2.0 / winHeight, 0.0);
    glVertex3f((x + h) * 2.0 / winWidth - 1.0, 1.0 - (y - h) * 2.0 / winHeight, 0.0);
    glVertex3f((x - h) * 2.0 / winWidth - 1.0, 1.0 - (y + h) * 2.0 / winHeight, 0.0);

    glEnd();
}

void chessBoardView::DrawCircle(int x, int y, int r)
{
    int winWidth = this->width();
    int winHeight = this->height();

    glBegin(GL_LINE_LOOP);

    for(int i = 0; i < 360; i++) {
        double rad = i*M_PI / 180;
        double dx = x + cos(rad) * r;
        double dy = y + sin(rad) * r;

        glVertex3f(dx * 2.0 / winWidth - 1.0, 1.0 - dy * 2.0 / winHeight, 0.0);
    }

    glEnd();
}

bool chessBoardView::JudgeWinner(int x, int y)
{
    if(m_chess[y][x] == 0) { return false; }
    //Hor
    {
        if(m_chess[y][0] == m_chess[y][1] && m_chess[y][1] == m_chess[y][2]) { return true; }
    }
    //Ver
    {
        if(m_chess[0][x] == m_chess[1][x] && m_chess[1][x] == m_chess[2][x]) { return true; }
    }
    //topleft-downright
    if(x == y) {
        if(m_chess[0][0] == m_chess[1][1] && m_chess[1][1] == m_chess[2][2]) { return true; }
    }
    //topright-downleft
    if(x + y == 2) {
        if(m_chess[0][2] == m_chess[1][1] && m_chess[1][1] == m_chess[2][0]) { return true; }
    }
    return false;
}

bool chessBoardView::CheckDraw()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(m_chess[j][i] == 0) { return false; }
        }
    }
    return true;
}

