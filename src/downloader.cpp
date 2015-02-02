#include "downloader.h"
#include "utilities.h"
#include "window.h"
#include "ui_mainwindow.h"


Downloader::Downloader(QObject *parent) : QObject(parent) {

}

void Downloader::read() {
    QString buffer = utils.downloadProcess->readAllStandardOutput();


    // This code if for handling progress bar
    QRegExp regexp("\\[download\\]\\s+(\\d+)\\.");
    regexp.indexIn(buffer);
    QString percent = regexp.capturedTexts()[1];
    if (percent != "")
        win.ui->downloadProgressBar->setValue(percent.toInt());

    if (buffer.contains("has already been downloaded"))
        win.ui->downloadProgressBar->setValue(100);

    utils.addToLog(buffer);
}

void Downloader::complete(int code) {
    if (utils.killed) {
        utils.addToLog("<b>Downloading canceled.</b>");
        utils.killed = false;
        return;
    }
    if (code != 0) {
        utils.addToLog("<b>Download error.</b>");
        return;
    }

    utils.addToLog("<b>Download complete</b>");
    utils.convert.start();
}

void Downloader::setCommand(QString c) {
    command = c;
}

void Downloader::start() {
    utils.downloadProcess = new QProcess;
    utils.downloadProcess->start(command);

    connect(utils.downloadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(read()));
    connect(utils.downloadProcess, SIGNAL(finished(int)), this, SLOT(complete(int)));
}
