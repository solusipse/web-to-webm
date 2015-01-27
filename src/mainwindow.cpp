#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    utils.ui = ui;
    utils.setTheme();
    utils.setCommons();
    utils.currentDownloadProcess = new QProcess;
    utils.currentConversionProcess = new QProcess;
    utils.lockAllControls(true);
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

    if (filename.isEmpty())
        return;

    utils.pathChanged = true;
    utils.currentFileName = filename;

    utils.setFilenameUI();
}

void MainWindow::on_stopConversion_clicked() {
    if (utils.currentDownloadProcess->atEnd() && utils.currentConversionProcess->atEnd())
        return;
    utils.killProcesses();
    utils.unlockConversionButton();
    utils.resetProgress();
}

void MainWindow::on_startConversion_clicked() {
    if (!utils.startProcedure())
        return;

    utils.resetProcesses();

    utils.ytFileName();
    utils.lockConversionButton();
    downloadProcess(utils.ytBinaryName() + " -f " + utils.ytGetQuality() + " -o " + utils.getCurrentRawFilename() + " " + utils.currentVideoUrl);
}

void MainWindow::downloadProcess(QString bin) {
    utils.currentDownloadProcess = new QProcess;
    utils.currentDownloadProcess->start(bin);

    connect(utils.currentDownloadProcess, SIGNAL(readyReadStandardOutput()), &utils, SLOT(downloadProcess()));
    connect(utils.currentDownloadProcess, SIGNAL(finished(int)), &utils, SLOT(downloadComplete(int)));
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
    utils.setFilenameUI();

    utils.addToLog("Changed resolution to: " + utils.currentQualityList[index][0]);
    //qDebug() << utils.currentQualityList[index];
    //ui->filenameEdit->setText(QDir().homePath() + "/" + QFileInfo(utils.ytFileName()).baseName() + ".webm");
}
