#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTextDocumentWriter>

// how to debug -> qdebug
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->markdownTextEdit->setPlainText("## Computer Graphics 1\nWelcome!");

    qDebug() << "This is a debug";
    qDebug() << ui->markdownTextEdit->toPlainText();

    updateRichText();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save() {

    qDebug() << "Would Save!";

    QString docPath  = QStandardPaths::locate(QStandardPaths::DocumentsLocation,
                                             "", QStandardPaths::LocateDirectory);

    qDebug() << "Documents path" << docPath;

    QString savePath = QFileDialog::getSaveFileName(this, "Save Markdown");

    qDebug() << "Save path" << savePath;


    if(!savePath.isEmpty()) {

        QTextDocumentWriter writer;
        writer.setFileName(savePath);

        // save from rich text view
        writer.write(ui->richTextEdit->document());
    }
}

void MainWindow::updateRichText()
{
    ui->richTextEdit->setMarkdown(ui->markdownTextEdit->toPlainText());
}
