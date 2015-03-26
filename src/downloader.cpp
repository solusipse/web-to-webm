#include "ui_mainwindow.h"
#include "downloader.h"
#include "utilities.h"
#include "window.h"

Downloader::Downloader(QObject *parent) : QObject(parent) {
}

void Downloader::start(QString c, QStringList l) {
    utils.downloadProcess = new QProcess;
    utils.downloadProcess->setProcessChannelMode(QProcess::MergedChannels);
    utils.downloadProcess->start(c, l);

    connect(utils.downloadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(read()));
    connect(utils.downloadProcess, SIGNAL(finished(int)), this, SLOT(complete(int)));
}

void Downloader::read() {
    QString buffer = utils.downloadProcess->readAllStandardOutput();

    QRegExp regexp("\\[download\\]\\s+(\\d+)\\.");
    regexp.indexIn(buffer);
    QString percent = regexp.capturedTexts()[1];
    if (percent != "")
        win.ui->downloadProgressBar->setValue(percent.toInt());

    if (buffer.contains("has already been downloaded"))
        win.ui->downloadProgressBar->setValue(100);

    if (win.ui->menuYoutubedlOutput->isChecked())
        utils.addToLog(buffer);
    else
        utils.addToLog(buffer, false);
}

void Downloader::complete(int code) {
    utils.downloadProcess->deleteLater();
    utils.downloadProcess = NULL;

    if (utils.killed) {
        utils.addToLog("<b>Downloading canceled.</b>");
        utils.killed = false;
        win.toggleConversionButton();
        return;
    }
    if (code != 0) {
        utils.addToLog("<b>Download error.</b>");
        win.toggleConversionButton();
        return;
    }

    utils.addToLog("<b>Download complete</b>");

    if (win.ui->menuDontConvert->isChecked()) {
        utils.addToLog("<b>User decided not to convert.</b> Downloaded video saved to: " + utils.getCurrentRawFilename());
        win.toggleConversionButton();
        return;
    }

    utils.convert.start();
}
