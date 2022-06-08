#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QTextDocument>

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


    int ret = pdfUtil.generatePdf(ui->inputLineEdit->text());

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

void MainWindow::print() {
    QString file = QDir::currentPath() + "/qt_text_pdf_test.pdf";
    qDebug() << "print: " << file;




#if 0
#if 1
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(file);
    QPainter painter;
    painter.begin(&printer);

    for (int page = 0; page < 1; ++page) {

        // Use the painter to draw on the page.
        printer.newPage();
    }

    painter.end();
#else
#if 0
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));
    if (true)
       dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
       return;
    }
#endif


    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);
    QPrintDialog*dlg = new QPrintDialog(&printer,this);
    dlg->setWindowTitle(QObject::tr("Print Document"));

    if(dlg->exec() == QDialog::Accepted) {
        parent.print(&printer);
    }
#endif
#else

    QTextDocument document;
    //document.setBaseUrl(QUrl("file://" + file));
    document.setHtml("Hello, World!\nLorem ipsum dolor sit amet, consectitur adipisci elit.");
#if 1

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("print_test.pdf");
    //printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    document.print(&printer);
#endif
#if 0
    QPrinter printer1(QPrinter::HighResolution);
    printer1.setOutputFormat(QPrinter::PdfFormat);
    printer1.setOutputFileName("scheda.pdf");
    QPrintDialog *dlg = new QPrintDialog(&printer1,this);
    dlg->setWindowTitle(QObject::tr("Print Document"));


    if(dlg->exec() == QDialog::Accepted) {

        document.print(&printer1);
    }
    delete dlg;
#endif




#endif
}
