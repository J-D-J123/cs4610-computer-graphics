#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void zoomReset();
    void zoomIn();
    void zoomOut();

private:

    // required
    void drawTestCases();
    void drawMidpointLine(int xStart, int yStart, int maxX, int maxY, QColor);

    // extra helpers
    int  checkRange(int x1, int y1, int x2, int y2);
    bool cohenSutherlandLineClip(int& x0, int& y0, int& x1, int& y1, int maxX, int maxY);
    int  computeOutCode(int x, int y, int maxX, int maxY);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
