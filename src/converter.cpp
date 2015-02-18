#include "ui_mainwindow.h"
#include "converter.h"
#include "utilities.h"
#include "window.h"
#include <QTime>

Converter::Converter(QObject *parent) : QObject(parent) {
}

void Converter::start() {
    utils.addToLog("<b>Starting conversion.</b>");
    utils.killProcesses();

    utils.currentDuration = 0;

    QStringList arguments;
    arguments << "-y" << "-hide_banner";
    arguments << "-i" << utils.getCurrentRawFilename();

    int bt = win.ui->bitrateValue->text().toInt();

    if (win.ui->menuRemoveAudio->isChecked())
        arguments << "-an";
    else
        bt -= 100;

    if (bt <= 0) bt = 1;
    QString bitrate = QString::number(bt);

    if (!bitrate.isEmpty()) {
        bitrate.append("K");
        arguments << "-b:v" << bitrate;
        utils.addToLog("Bitrate set to: " + bitrate);
    }

    if (!utils.configGetValue("ffmpeg_params").isEmpty()) {
        arguments << utils.configGetValue("ffmpeg_params").split(" ");
        utils.addToLog("Used custom parameters: " + utils.configGetValue("ffmpeg_params"));
    }

    if (!win.ui->cutFromEdit->text().trimmed().isEmpty() && !win.ui->cutToEdit->text().trimmed().isEmpty()) {
        arguments << "-ss" << win.ui->cutFromEdit->text().trimmed();
        arguments << "-to" << win.ui->cutToEdit->text().trimmed();
        utils.addToLog("Output video length: " + (QTime(0,0,0).addSecs(utils.getTrimmedVideoDuration())).toString("hh:mm:ss"));
        utils.currentDuration = utils.getTrimmedVideoDuration();
    }

    arguments << utils.getCurrentFilename();

    utils.conversionProcess = new QProcess;
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
    if (buffer.contains("Duration: ") && utils.currentDuration == 0) {
        duration = time.capturedTexts()[0];
        durationTime = QTime::fromString(duration, "hh:mm:ss.z");
        utils.currentDuration = QTime(0,0).secsTo(durationTime);
    } else {
        if (buffer != "") {
            progress = time.capturedTexts()[0];
            if (progress != "") {
                progressTime = QTime::fromString(progress, "hh:mm:ss.z");
                int percent = double((QTime(0,0).msecsTo(progressTime)) / double(utils.currentDuration*1000))*100;
                win.ui->conversionProgressBar->setValue(percent);
            }
        }
    }
    if (win.ui->menuFfmpegOutput->isChecked())
        utils.addToLog(buffer);
    else
        utils.addToLog(buffer, false);
}

void Converter::complete(int code) {
    utils.conversionProcess->deleteLater();
    utils.conversionProcess = NULL;
    win.updateConversionButton();

    if (utils.killed) {
        utils.addToLog("<b>Conversion canceled.</b>");
        utils.killed = false;
        win.toggleConversionButton();
        return;
    }
    if (code != 0) {
        utils.addToLog("<b>Error on conversion. Check logs.</b>");
        win.toggleConversionButton();
        return;
    }

    // In case video is so short that ffmpeg do not display
    // conversion status
    win.ui->conversionProgressBar->setValue(100);

    win.toggleConversionButton();
    win.updateConversionButton();

    if (win.ui->menuRemoveRawVideo->isChecked())
        utils.removeRawVideo();

    utils.addToLog("<b>Conversion complete.</b>");
    utils.addToLog("Saved to: " + utils.getCurrentFilename());

    if (win.ui->menuShowFile->isChecked())
        utils.showFileInDirectory();
}
