#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "utilities.h"
#include "window.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopServices>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    win.ui = ui;

    win.setTheme();
    utils.setCommons();
    utils.configInit();
    win.lockAllControls(true);
    win.setPlayerHtml();
}

MainWindow::~MainWindow() {
    // TODO: save settings
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

void MainWindow::on_startConversion_clicked() {
    win.toggleConversionButton();
    win.resetProgress();
    if (ui->startConversion->isChecked()) {
        if (!utils.checkUrl())
            return;
        utils.getFileName();

        QStringList arguments;
        arguments << "-f" << utils.getVideoQuality() << "-o";
        arguments << utils.getCurrentRawFilename() << utils.currentVideoUrl;

        utils.download.start(utils.getBinaryName(), arguments);
    } else {
        utils.killProcesses();
    }
}

void MainWindow::on_menuAbout_triggered()
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
    win.resetProgress();
    utils.addToLog("Changed resolution to: " + utils.currentQualityList[index][0]);
}

void MainWindow::on_menuWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/solusipse/ytwebm"));
}

void MainWindow::on_menuDefaultDownloadPath_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        QDir().homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        utils.configSetValue("default_path", dir);
}

void MainWindow::on_menuExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_menuClearLog_triggered() {
    win.ui->logBox->clear();
}

void MainWindow::on_menuShowLog_triggered()
{
    QDialog dialog(this, Qt::Window);
    QPlainTextEdit *box = new QPlainTextEdit(&dialog);
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.resize(640, 480);
    dialog.setWindowTitle("ytwebm log");
    layout->addWidget(box);
    dialog.setLayout(layout);
    box->setReadOnly(true);
    box->setPlainText(utils.log);
    dialog.exec();
}
