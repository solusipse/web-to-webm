#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

#include <QFileDialog>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    utils.ui = ui;
    utils.setTheme();
    utils.setCommons();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_urlEdit_returnPressed() {
    utils.setVideoDetails(ui->urlEdit->text());
}

void MainWindow::on_selectSavePath_clicked() {
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Select location for converted film"), QDir::currentPath(), tr("Webm (*.webm)") );

    if (!filename.isEmpty())
        ui->filenameEdit->setText(filename);
}

void MainWindow::on_stopConversion_clicked() {
    if (utils.currentDownloadProcess->atEnd())
        utils.currentDownloadProcess->close();
}

void MainWindow::on_startConversion_clicked() {
    if (!utils.startProcedure())
        return;

    if (utils.currentDownloadProcess->atEnd())
        utils.currentDownloadProcess->close();

    downloadProcess("youtube-dl.exe " + utils.currentVideoUrl);
}

void MainWindow::downloadProcess(QString bin) {
    utils.currentDownloadProcess = new QProcess;
    utils.currentDownloadProcess->start(bin);

    connect(utils.currentDownloadProcess, SIGNAL(readyReadStandardOutput()), &utils, SLOT(downloadProgress()));
}
