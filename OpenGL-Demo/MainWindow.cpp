#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_viewWidget = new ViewWidget(this);
    setCentralWidget(m_viewWidget);

    setWindowTitle("Assignment 4: OpenGL");
}

MainWindow::~MainWindow()
{
    delete ui;
}
