#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), pdfUtil()
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::generatePDF() {
    qDebug() << "generatePDF: " << QDir::currentPath();

    int ret = pdfUtil.generatePdf("test abc");

    qDebug() << "generatePdf ret: " << ret;
}
