#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMatrix4x4>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// struct for triangle points and colors needed for rendering
struct Triangle
{
    QPointF v1;
    QPointF v2;
    QPointF v3;

    QColor c1;
    QColor c2;
    QColor c3;
};

// struct for lines in 3D
struct Line3D
{
    QVector3D v1;
    QVector3D v2;
    QColor color;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawScene();

public slots:
    void zoomReset();
    void zoomIn();
    void zoomOut();

private:

    QMatrix4x4 m_view;
    QMatrix4x4 m_model;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_viewport;

    // required
    void floodFill(int x, int y, const QColor& fill);
    void drawTriangleLines(const Triangle &triangle, const QColor& color);
    void drawTriangleFlood(const Triangle &triangle, const QColor &color);
    void drawTriangleFilled(const Triangle &triangle);
    void drawLines3D(const QVector<Line3D> &lines, const QMatrix4x4 &modelViewProjection);

    // helper
    float edgeFunction(int ax, int ay, int bx, int by, int cx, int cy);
    Triangle generateRandomTriangle(const QPoint& min, const QPoint& max);

    // from midpoint line
    void drawTestCases();
    void drawMidpointLine(int xStart, int yStart, int maxX, int maxY, QColor);
    int  checkRange(int x1, int y1, int x2, int y2);
    bool cohenSutherlandLineClip(int& x0, int& y0, int& x1, int& y1, int maxX, int maxY);
    int  computeOutCode(int x, int y, int maxX, int maxY);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
