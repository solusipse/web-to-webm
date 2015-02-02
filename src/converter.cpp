#include "converter.h"
#include "utilities.h"
#include "window.h"
#include "ui_mainwindow.h"

Converter::Converter(QObject *parent) : QObject(parent) {
    utils.conversionProcess = new QProcess;
}

void Converter::start() {
    utils.addToLog("<b>Starting conversion.</b>");

    utils.killProcesses();

    QStringList arguments;
    arguments << "-y" << "-hide_banner" << "-i" << utils.getCurrentRawFilename() << utils.getCurrentFilename();

    utils.conversionProcess->setProcessChannelMode(QProcess::MergedChannels);
    utils.conversionProcess->start(utils.ffmpegBinaryName(), arguments);

    connect(utils.conversionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(read()));
    connect(utils.conversionProcess, SIGNAL(finished(int)), this, SLOT(complete(int)));
}

void Converter::read() {
    QString buffer = utils.conversionProcess->readAllStandardOutput();
    QString duration, progress;
    QTime durationTime, progressTime;
    QRegExp time("\\d\\d\\:\\d\\d\\:\\d\\d\\.\\d\\d");
    time.indexIn(buffer);
    if (buffer.contains("Duration: ")) {
        duration = time.capturedTexts()[0];
        durationTime = QTime::fromString(duration, "hh:mm:ss.z");
        utils.currentDuration = QTime(0,0).secsTo(durationTime);
    } else {
        if (buffer != "") {
            progress = time.capturedTexts()[0];
            progressTime = QTime::fromString(progress, "hh:mm:ss.z");
            int percent = double((QTime(0,0).secsTo(progressTime)) / double(utils.currentDuration))*100;
            win.ui->conversionProgressBar->setValue(percent);
        }
    }
    utils.addToLog(buffer);
}

void Converter::complete(int code) {
    if (utils.killed) {
        utils.addToLog("<b>Conversion canceled.</b>");
        utils.killed = false;
        return;
    }
    if (code != 0) {
        utils.addToLog("<b>Error on conversion. Check logs.</b>");
        return;
    }

    utils.addToLog("<b>Conversion complete.</b>");
    utils.addToLog("Saved to: " + utils.getCurrentFilename());

    win.lockConversionButton(false);
}
