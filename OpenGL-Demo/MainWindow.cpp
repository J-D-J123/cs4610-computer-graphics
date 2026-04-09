#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Assignment 4: OpenGL");

    m_viewWidget = new ViewWidget(this);
    setCentralWidget(m_viewWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
