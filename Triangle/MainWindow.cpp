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
 * @param x1 start p(x1
 * @param y1 satrt p   , y1)
 * @param x2 start p(x2
 * @param y2 start p   , y2
 */
void MainWindow::drawMidpointLine(int x1, int y1, int x2, int y2, QColor color) {

    // if start point is outside the bounds just return
    // bool inRange = checkRange(x1, y1, x2, y2);

    int maxX = ui->pixelWidget->bufferSize().width()   - 1;
    int maxY = ui->pixelWidget->bufferSize().height()  - 1;

    // call the CohenSutherland algorithm and see if line is outside frame
    bool accepts = !cohenSutherlandLineClip(x1, y1, x2, y2, maxX, maxY);

    // if the points are outside the display clip them if not return
    if (accepts) {
        return;
    }

    // if(inRange) {
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
    // } else {

        // qDebug() << "Error: Line Outside Bounds Unable to Draw"
        //          << "(" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")";
    // }
}

/**
 * @brief checkRange returns 0 if range is inside the window else it returns 1
 * @param x1 start p(x1
 * @param y1 satrt p   , y1)
 * @param x2 start p(x2
 * @param y2 start p   , y2
 * @return
 */
int MainWindow::checkRange(int x1, int y1, int x2, int y2) {

    int maxX = ui->pixelWidget->bufferSize().width()  - 1;
    int maxY = ui->pixelWidget->bufferSize().height() - 1;

    // qDebug() << "Screen Size is:"
    //          << "(" << maxX << "x" << maxY << ")";

    if  (x1 < 0 || x1 > maxX ||
            y1 < 0 || y1 > maxY ||
            x2 < 0 || x2 > maxX ||
        y2 < 0 || y2 > maxY) {

        return 0;
    } else {return 1;}
}


/**
 * @brief MainWindow::cohenSutherLandAlgo computes the bit code for a point (x, y)
 * @return the int of the region where the point is in
 * Resources:
 * https://www.tutorialspoint.com/computer_graphics/computer_graphics_cohen_sutherland_line_clipping.htm
 * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 */
int MainWindow::computeOutCode(int x, int y, int maxX, int maxY) {

    // right left top bottom
    // copied from Wikipedia
    // const int INSIDE = 0b0000;
    const int LEFT   = 0b0001;
    const int RIGHT  = 0b0010;
    const int BOTTOM = 0b0100;
    const int TOP    = 0b1000;

    int code = 0;

    if (x < 0 ) {

        code |= LEFT;

    } else if (x > maxX) {

        code |= RIGHT;
    }

    if (y < 0) {

        code |= TOP;

    } else if (y > maxY) {

        code |= BOTTOM;
    }

    return code;
}

bool MainWindow::cohenSutherlandLineClip(int& x0, int& y0, int& x1, int& y1, int maxX, int maxY) {

    // right left top bottom
    // copied from Wikipedia
    // const int INSIDE = 0b0000;
    const int LEFT   = 0b0001;
    const int RIGHT  = 0b0010;
    const int BOTTOM = 0b0100;
    const int TOP    = 0b1000;


    // get the (x, y) cords
    int outcodeStart = computeOutCode(x0, y0, maxX, maxY);
    int outcodeEnd = computeOutCode(x1, y1, maxX, maxY);

    bool accept = false;

    while (true) {
        if (!(outcodeStart | outcodeEnd)) {
            // both are inside
            accept = true;
            break;
        } else if (outcodeStart & outcodeEnd) {
            // both are outside
            break;
        } else {
            // failed both tests, so caculate point (x, y)
            int x, y;
            int outcodeOutput = outcodeEnd > outcodeStart ? outcodeEnd : outcodeStart;

            // now find the intersection point
            // use formula: slope = (y1 - y0) / (x1 - x0)
            //  x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //  y = y0 + slope * (xm - x0), where xm is xmin or xmax
            if (outcodeOutput & TOP) {
                // point is above clip window, y = 0 is top not bottom
                x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
                y = 0;
            } else if (outcodeOutput & BOTTOM) { // point is below the clip window
                x = x0 + (x1 - x0) * (maxY - y0) / (y1 - y0);
                y = maxY;
            } else if (outcodeOutput & RIGHT) {  // point is to the right of clip window
                y = y0 + (y1 - y0) * (maxX - x0) / (x1 - x0);
                x = maxX;
            } else if (outcodeOutput & LEFT) {   // point is to the left of clip window
                y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
                x = 0;
            }

            // move outside point to intersection point to clip
            if (outcodeOutput == outcodeStart) {
                x0 = x;
                y0 = y;
                outcodeStart = computeOutCode(x0, y0, maxX, maxY);
            } else {
                x1 = x;
                y1 = y;
                outcodeEnd = computeOutCode(x1, y1, maxX, maxY);
            }
        }
    }
    return accept;
}

void MainWindow::drawTestCases() {

    // put data here to draw on the screen (the test cases)
    // vertical lines, horizontal lines

    // // horizontal line
    //  drawMidpointLine(10, 10, 100, 10, Qt::red);

    // // verticial line
    // drawMidpointLine(50, 10, 50, 100, Qt::blue);

    // // diagonals, x-major iteration & y
    // drawMidpointLine(10, 10, 100, 100, Qt::green);
    // drawMidpointLine(0, 0, 511, 511, Qt::blue);

    // // left to right x1 < x2
    // drawMidpointLine(10, 20, 100, 20, Qt::yellow);

    // // right to left x1 > x2
    // drawMidpointLine(100, 40, 10, 40, Qt::black);

    // top to bottom y1 < y2
    // drawMidpointLine(200, 10, 200, 100, Qt::cyan);

    // bottom to top y1 > y2
    // drawMidpointLine(250, 100, 250, 10, Qt::magenta);
}

