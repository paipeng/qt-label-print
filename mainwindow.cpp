#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

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

    if (ret == 0) {
        QDesktopServices::openUrl(QUrl("file://" + QDir::currentPath() + "/qt_text_pdf_test.pdf"));
        QFile file(QDir::currentPath() + "/qt_text_pdf_test.pdf");

        qDebug() << "file: " << file.fileName();
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
    }
}
