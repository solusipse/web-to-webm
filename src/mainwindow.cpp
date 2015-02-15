#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "utilities.h"
#include "window.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    win.ui = ui;

    this->setWindowTitle("web-to-webm v." VERSION);

    win.setTheme();
    utils.setCommons();
    utils.configInit();
    utils.configLoadAll();
    win.lockAllControls(true);
    win.setPlayerHtml();
    utils.addToLog("Initialised.");
}

MainWindow::~MainWindow() {
    utils.configSaveAll();
    utils.killProcesses();
    delete ui;
}

void MainWindow::on_urlEdit_returnPressed() {
    utils.loadVideo(win.ui->urlEdit->text());
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
    win.updateConversionButton();
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

void MainWindow::on_menuAbout_triggered() {
    QMessageBox::about(this, "web-to-webm about",
        "web-to-webm v.0.6.0\n\n"
        "This software is open source (MIT licensed). "
        "It was build with QT (LGPL), youtube-dl (Public Domain) and ffmpeg (LGPL).\n\n"
        "For more informations see:\n"
        "https://github.com/solusipse/web-to-webm.");
}

void MainWindow::on_qualityComboBox_currentIndexChanged(int index) {
    win.setFilename();
    win.resetProgress();
    utils.addToLog("Changed resolution to: " + utils.currentQualityList[index][0]);
}

void MainWindow::on_menuWebsite_triggered() {
    QDesktopServices::openUrl(QUrl("https://github.com/solusipse/web-to-webm"));
}

void MainWindow::on_menuDefaultDownloadPath_triggered() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        QDir().homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        utils.configSetValue("default_path", dir);
}

void MainWindow::on_menuExit_triggered() {
    qApp->exit();
}

void MainWindow::on_menuClearLog_triggered() {
    win.ui->logBox->clear();
}

void MainWindow::on_menuShowLog_triggered() {
    QDialog dialog(this, Qt::Window);
    QPlainTextEdit *box = new QPlainTextEdit(&dialog);
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.resize(640, 480);
    dialog.setWindowTitle("web-to-webm log");
    layout->addWidget(box);
    dialog.setLayout(layout);
    box->setReadOnly(true);
    box->setPlainText(utils.log);
    dialog.exec();
}

void MainWindow::on_menuNew_triggered() {
    utils.killProcesses();
    win.setPlayerHtml();
    win.ui->titleEdit->clear();
    win.ui->urlEdit->clear();
    win.reset();
}

void MainWindow::on_menuCustomYoutubedlPath_triggered() {
    bool ok;
    QString path = QInputDialog::getText(this,
            tr("Set custom youtube-dl path"),
            tr("youtube-dl <b>absolute</b> path (leave blank for default):"), QLineEdit::Normal,
            utils.configGetValue("youtubedl_path"), &ok);
    if (ok)
        utils.configSetValue("youtubedl_path", path);
}

void MainWindow::on_menuCustomFfmpegPath_triggered() {
    bool ok;
    QString path = QInputDialog::getText(this,
            tr("Set custom ffmpeg path"),
            tr("ffmpeg <b>absolute</b> path (leave blank for default):"), QLineEdit::Normal,
            utils.configGetValue("ffmpeg_path"), &ok);
    if (ok)
        utils.configSetValue("ffmpeg_path", path);
}

void MainWindow::on_menuResetAllSettings_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Reset settings", "Are you sure you want to reset all settings?",
        QMessageBox::Yes|QMessageBox::No);

    if (reply != QMessageBox::Yes)
      return;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "solusipse", "web-to-webm");
    for (int i = 0; i < settings.allKeys().length(); i++)
        settings.remove(settings.allKeys()[i]);
    utils.configInit();
}

void MainWindow::on_menuFfmpegCustomParams_triggered() {
    bool ok;
    QString path = QInputDialog::getText(this,
            tr("Set custom ffmpeg parameters"),
            tr("youtube-dl ffmpeg (e.g. -b:v 2M, leave blank for default):"), QLineEdit::Normal,
            utils.configGetValue("ffmpeg_params"), &ok);
    if (ok)
        utils.configSetValue("ffmpeg_params", path);
}
