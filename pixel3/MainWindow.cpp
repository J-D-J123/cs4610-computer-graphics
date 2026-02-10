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
    for (int x = 0; x < ui->pixelWidget->bufferSize().width(); ++x) {

        // Gray Center
        ui->pixelWidget->writePixel(x, x, palette().windowText().color());

        // Draw adjacent diagonal in red
        ui->pixelWidget->writePixel(x + 1, x, Qt::red);

        // draw adjacent diagonal in green
        ui->pixelWidget->writePixel(x, x + 1, QColor(0,255,0));
    }

    // Now implemen Bresenham's Line Drawing Algorithm
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
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
