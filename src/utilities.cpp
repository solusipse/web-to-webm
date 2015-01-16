#include "utilities.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QWebSettings>
#include <QProcess>
#include <QDir>


Utilities utils;

Utilities::Utilities(){
}

void Utilities::setTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window, QColor("#333"));
    p.setColor(QPalette::Button, QColor("#333"));
    p.setColor(QPalette::Highlight, QColor("#b31217"));
    p.setColor(QPalette::ButtonText, QColor(155,155,155));
    p.setColor(QPalette::WindowText, QColor("#888"));
    p.setColor(QPalette::Light, QColor("#333"));
    qApp->setPalette(p);
    setStylesheet();
}

void Utilities::setStylesheet() {
    const char *css = "QPushButton:hover{background:#7d1cb4;}"
                      "QWebView{background:#333;} QPlainTextEdit{background:#333;}"
                      "QLineEdit{background:#888;border:none;height:35px;padding-left:10px;color:#222;}"
                      "QProgressBar{background:#888; border:none;height:35px;color:#333;} ";

    qApp->setStyleSheet(css);
}

void Utilities::setCommons() {
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    utils.ui->player->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
}

void Utilities::setVideoDetails(QString url) {
    url = ytPrepareUrl(url);

    if (url == "error") {
        utils.ui->titleEdit->setText("Error: provided url is incorrect.");
        return;
    }
    utils.ui->player->load(url);
    utils.ui->titleEdit->setText(ytVideoTitle(url));

    setFilename(url);
}

QString Utilities::execBinary(QString bin) {
    QProcess program;
    program.start(bin);
    while(program.waitForFinished(-1))
        return program.readAllStandardOutput().simplified();
    return "Error on executing " + bin;
}

QString Utilities::ytVideoTitle(QString url) {
    return execBinary("youtube-dl.exe -e " + url);
}

QString Utilities::ytVideoID(QString url) {
    return execBinary("youtube-dl.exe --get-id " + url);
}

QString Utilities::ytPrepareUrl(QString url) {
    url = execBinary("youtube-dl.exe --get-id " + url);
    if (url.isEmpty())
        return "error";
    return "https://www.youtube.com/embed/" + url;
}

void Utilities::setFilename(QString url) {
    QString ytid = execBinary("youtube-dl.exe --get-id " + url);
    utils.ui->filenameEdit->setText(QDir().homePath() + "/" + ytid + ".webm");
}
