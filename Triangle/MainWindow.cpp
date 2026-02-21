#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QThread>

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

/**
 * @brief MainWindow::drawTriangleLines
 * @param triangle
 * @param color
 */
void MainWindow::drawTriangleLines(const Triangle &triangle, const QColor& color) {

    // now make loop to test lots of cases (replace this with a loop)
    drawMidpointLine(triangle.v1.x(), triangle.v1.y(),
                     triangle.v2.x(), triangle.v2.y(), color);

    drawMidpointLine(triangle.v2.x(), triangle.v2.y(),
                     triangle.v3.x(), triangle.v3.y(), color);

    drawMidpointLine(triangle.v1.x(), triangle.v1.y(),
                     triangle.v3.x(), triangle.v3.y(), color);

}

/**
 * @brief MainWindow::floodFill fills the triangle recursively with a color
 * @param x the x cord of where the point p starts
 * @param y the y cord of where the point p starts
 * @param fill is the color of the fill
 */
void MainWindow::floodFill(int x, int y, const QColor& fill)
{
    int maxX = ui->pixelWidget->bufferSize().width()  - 1;
    int maxY = ui->pixelWidget->bufferSize().height() - 1;

    // step 1: if node is not Inside, return
    // "Inside" means: in bounds AND not already filled AND not a border pixel
    if (x < 0 || x > maxX || y < 0 || y > maxY) {
        return;
    }

    QColor current = ui->pixelWidget->readPixel(x, y);

    // not inside if already fill color
    if (current != QColor(0, 0, 0, 0)) {
        return;
    }

    // step 2: set the node
    ui->pixelWidget->writePixel(x, y, fill);

    // steps 3-6: recurse in 4 directions
    floodFill(x,     y + 1, fill); // south
    floodFill(x,     y - 1, fill); // north
    floodFill(x - 1, y,     fill); // west
    floodFill(x + 1, y,     fill); // east
}

/**
 * drawTestCases() tests mutliple test cases on where to draw triangles
 * @brief MainWindow::drawTestCases
 */
void MainWindow::drawTestCases() {

    Triangle triangle1{{100, 100},
                       {500, 200},
                       {300, 400},
                       Qt::red,
                       Qt::green,
                       Qt::blue};

    // THIS FINALLY WORKS!
    drawTriangleLines(triangle1, Qt::red);

    qDebug() << ui->pixelWidget->readPixel(300, 250);

    // make the stack so my computer does not crash
    QThread* thread = QThread::create([this](){
        floodFill(300, 250, Qt::blue);
    });
    thread->setStackSize(64 * 1024 * 1024); // 64MB stack
    thread->start();
    thread->wait();
    delete thread;




    // faaaaahhhhhhhh
    // int maxX = ui->pixelWidget->bufferSize().width()  - 1;
    // int maxY = ui->pixelWidget->bufferSize().height() - 1;

    // // reset per run
    // srand(time(0));

    // // make 200 random generated triangles
    // for(int i = 0; i < 200; i++) {

    //     // choose random x, y chords for vertex 1, 2 and 3
    //     int x1 = rand() % maxX;
    //     int x2 = rand() % maxX;
    //     int x3 = rand() % maxX;

    //     int y1 = rand() % maxY;
    //     int y2 = rand() % maxY;
    //     int y3 = rand() % maxY;

    //     Triangle triangle1{{(qreal) x1, (qreal)y1},  // vertex 1
    //                        {(qreal) x2, (qreal) y2},  // vertex 2
    //                        {(qreal) x3, (qreal) y3},  // vertex 3
    //                        Qt::red,     // color 1
    //                        Qt::green,   // color 2
    //                        Qt::blue};   // color 3



    //     // drawMidpointLine(triangle1.v1.x(), triangle1.v1.y(),
    //     //                  triangle1.v2.x(), triangle1.v2.y(), triangle1.c1);

    //     // drawMidpointLine(triangle1.v2.x(), triangle1.v2.y(),
    //     //                  triangle1.v3.x(), triangle1.v3.y(), triangle1.c2);

    //     // drawMidpointLine(triangle1.v1.x(), triangle1.v1.y(),
    //     //                  triangle1.v3.x(), triangle1.v3.y(), triangle1.c3);
}

