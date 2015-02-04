#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"
#include "window.h"
#include "downloader.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    win.ui = ui;

    win.setTheme();
    utils.setCommons();
    win.lockAllControls(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_urlEdit_returnPressed() {
    win.setVideoDetails(win.ui->urlEdit->text());
}

void MainWindow::on_selectSavePath_clicked() {
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Select location for converted film"), QDir::currentPath(), tr("Webm (*.webm)") );

    if (filename.isEmpty())
        return;

    utils.pathChanged = true;
    utils.currentFileName = filename;

    win.setFilename();
}

void MainWindow::on_stopConversion_clicked() {
    utils.killProcesses();
    win.lockConversionButton(false);
    win.resetProgress();
}

void MainWindow::on_startConversion_clicked() {
    if (!utils.startProcedure())
        return;

    //utils.resetProcesses();

    utils.ytFileName();
    win.lockConversionButton(true);

    utils.download.setCommand(utils.ytBinaryName() + " -f " + utils.ytGetQuality() + " -o " + utils.getCurrentRawFilename() + " " + utils.currentVideoUrl);
    utils.download.start();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "ytwebm about", "This software is open source (MIT licensed). "
                                             "It was build with QT (LGPL), youtube-dl (Public Domain) and ffmpeg (LGPL).\n\n"
                                             "For more informations see:\n"
                                             "https://github.com/solusipse/ytwebm.");
}

void MainWindow::on_qualityComboBox_currentIndexChanged(int index)
{
    if (utils.loadingVideoInformations)
        return;

    win.setFilename();
    utils.addToLog("Changed resolution to: " + utils.currentQualityList[index][0]);
}
