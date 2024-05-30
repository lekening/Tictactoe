#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H
#include <QOpenGLWidget>

class chessBoardView: public QOpenGLWidget
{
    Q_OBJECT

public:
    chessBoardView(QWidget* parent = nullptr);

    bool addChess(int x, int y, bool circle);
    int testChessPos(int x, int y, bool circle);
    void reset();

signals:
    void NewChessPosition(int x, int y);
    void HaveWinner(int i);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent* mEvent);
    void mouseReleaseEvent(QMouseEvent* mEvent);



private:
    void DrawCross(int x, int y, int h);
    void DrawCircle(int x, int y, int r);


    bool JudgeWinner(int x, int y);
    bool CheckDraw();

    int m_chess[3][3];

};



#endif // CHESSBOARDVIEW_H
