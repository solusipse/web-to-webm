#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    utils.ui = ui;
    utils.setTheme();
    utils.setCommons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_urlEdit_returnPressed()
{
    utils.setVideoDetails(ui->urlEdit->text());
}


QString MainWindow::prepareUrl(QString url) {
    if (url.startsWith("http://") || url.startsWith("https://"))
        return url;
    else
        url.prepend("https://");

    //if (!url.startsWith("https://youtu"))
    //    return "error";

    return url;
}

void MainWindow::on_selectSavePath_clicked() {
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Select location for converted film"), QDir::currentPath(), tr("Webm (*.webm)") );

    ui->filenameEdit->setText(filename);
}

void MainWindow::on_stopConversion_clicked() {

}

void MainWindow::on_startConversion_clicked() {

}
