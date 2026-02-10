#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionActual_Size, &QAction::triggered, this, &MainWindow::zoomReset);
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);

    // clear the pixels
    ui->pixelWidget->clear();

    // loop through the entire pixels
    // for (int x = 0; x < ui->pixelWidget->bufferSize().width(); ++x) {

    //     // Gray Center
    //     ui->pixelWidget->writePixel(x, x, palette().windowText().color());

    //     // Draw adjacent diagonal in red
    //     ui->pixelWidget->writePixel(x + 1, x, Qt::red);

    //     // draw adjacent diagonal in green
    //     ui->pixelWidget->writePixel(x, x + 1, QColor(0,255,0));
    // }

    // Now implemen Bresenham's Line Drawing Algorithm
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    // Integer-only

    // function for writing a pixel inside the line is:
    // PixelWidget::writePixel(int x, int y, const QColor& c)

    // Cases ... if f(x, y) > 0 then positve, if f(x, y) < 0 then negative.

    drawTestCases();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::zoomReset() {

    // qDebug() << "zoomReset()";
    ui->pixelWidget->setZoom(1);
}

void MainWindow::zoomIn() {

    // qDebug() << "zoomIn()";
    ui->pixelWidget->setZoom(ui->pixelWidget->zoom() * 2);
}

void MainWindow::zoomOut() {

    // qDebug() << "zoomOut()";
    ui->pixelWidget->setZoom(ui->pixelWidget->zoom() * 0.5);
}

/**
 * @brief MainWindow::drawMidpointLine
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void MainWindow::drawMidpointLine(int x1, int y1, int x2, int y2, QColor color) {

    // if start point is outside the bounds just return
    bool inRange = checkRange(x1, y1, x2, y2);

    if(inRange) {
        // now draw the line started at f(x1, y1) to f(x2, y2)
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        // function for writing a pixel inside the line is:
        // PixelWidget::writePixel(int x, int y, const QColor& c)

        // step 1 figure out deltas
        int deltaY = y2 - y1;
        int deltaX = x2 - x1;


        // f(x,y) = Ax + By + C = 0 where;
        // A = deltaY = y1 - y0
        // B = -deltaX = -(x1 - x0)
        // C = (deltaX)b = (x1-x0)b

        // step 2 figure out which direction to step
        // if deltaX > 0 step right if negative step left
        // same for deltaY
        int stepX = (deltaX > 0) ? 1 : -1;
        int stepY = (deltaY > 0) ? 1 : -1;

        // take abs so it does not go crazy negative values
        deltaX    = abs(deltaX);
        deltaY    = abs(deltaY);

        // step 3 slope error
        int slopeErr = deltaX - deltaY;

        int x = x1;
        int y = y1;

        // step 4 walk pixel by pixel until (x2, y2)
        while(true) {
            ui->pixelWidget->writePixel(x, y, color);

            if (x == x2 && y == y2) {
                break;
            } else {

                int slopeNewError = 2 * slopeErr;

                if (slopeNewError > -deltaY) {
                    slopeErr -= deltaY;
                    x        += stepX;
                }

                if (slopeNewError < deltaX) {

                    slopeErr += deltaX;
                    y        += stepY;
                }

            }
        }
    }
    qDebug() << "Error: Line Outside Bounds Unable to Draw";
}

/**
 * @brief checkRange returns 0 if range is inside the window else it returns 1
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @return
 */
int MainWindow::checkRange(int x1, int y1, int x2, int y2) {

    int maxX = ui->pixelWidget->bufferSize().width()  - 1;
    int maxY = ui->pixelWidget->bufferSize().height() - 1;

    if  (x1 < 0 || x1 > maxX ||
            y1 < 0 || y1 > maxY ||
            x2 < 0 || x2 > maxX ||
        y2 < 0 || y2 > maxY) {

        return 0;
    } else {return 1;}
}

void MainWindow::drawTestCases() {

    // put data here to draw on the screen (the test cases)
    // vertical lines, horizontal lines

    // horizontal line
     drawMidpointLine(10, 10, 100, 10, Qt::red);

    // verticial line
    drawMidpointLine(50, 10, 50, 100, Qt::blue);

    // diagonals, x-major iteration & y
    drawMidpointLine(10, 10, 100, 100, Qt::green);
    drawMidpointLine(-10, -10, -100, -100, Qt::blue);

    // left to right x1 < x2
    drawMidpointLine(10, 20, 100, 20, Qt::yellow);

    // right to left x1 > x2
    drawMidpointLine(100, 40, 10, 40, Qt::black);

    // top to bottom y1 < y2
    // bottom to top y1 > y2

    // use diff colors for each case
}
