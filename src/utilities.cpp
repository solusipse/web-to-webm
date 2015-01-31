#include "utilities.h"
#include "window.h"
#include "ui_mainwindow.h"
#include <QWebSettings>
#include <QProcess>
#include <QDir>
#include <QTime>


Utilities utils;

Utilities::Utilities(){
}

void Utilities::setCommons() {
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    win.ui->player->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

}


QString Utilities::execBinary(QString bin, int multiline = 0) {
    QProcess program;
    program.start(bin);
    while(program.waitForFinished(-1)) {
        if (multiline == 0)
            return QString::fromLocal8Bit(program.readAllStandardOutput().simplified());
        return program.readAllStandardOutput();
    }
    return "Error on executing " + bin;
}

QString Utilities::ytVideoTitle(QString url) {
    return execBinary(ytBinaryName() + " -e " + url);
}

QString Utilities::ytVideoID(QString url) {
    currentID = execBinary(ytBinaryName() + " --get-id " + url);
    return currentID;
}

QString Utilities::ytPrepareUrl(QString url) {
    url = ytVideoID(url);
    if (url.isEmpty())
        return "error";
    return "https://www.youtube.com/embed/" + url;
}

void Utilities::addToLog(QString line) {
    // TODO: add saving log to file option
    win.ui->logBox->appendHtml(line);
}

bool Utilities::startProcedure() {
    if (currentVideoUrl.isEmpty()) {
        addToLog("Add valid video first.");
        return false;
    }
    addToLog("<br><b>Started downloading video:</b><br>" + win.ui->urlEdit->text());
    return true;
}

void Utilities::downloadProcess() {
    QString buffer = utils.currentDownloadProcess->readAllStandardOutput();

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

QVector< QVector<QString> > Utilities::ytQualityList(QString url) {
    QVector< QVector<QString> > list;
    QString formats = execBinary(ytBinaryName() + " -F " + url, 1);
    QStringList formatsList = formats.split("\n");

    /*
     * there are always clips in webm and mp4 with same resolution
     * since mp4 provides better quality and there is sometimes
     * higher quality in mp4 format available, script is excluding webm
     * at this moment
    */

    // TODO: add support for other sites than yt
    // full list of supported sites:
    // http://rg3.github.io/youtube-dl/supportedsites.html

    for (int i = formatsList.length()-1; i >= 0 ; i--) {
        if (!formatsList[i].contains("webm"))
            if (!formatsList[i].contains("audio only"))
                if (!formatsList[i].contains("video only")) {
                    QRegExp resolution("\\d{3,4}x\\d{3}");
                    resolution.indexIn(formatsList[i]);
                    QRegExp code("\\d\\d");
                    code.indexIn(formatsList[i]);
                    QRegExp format("\\w\\w\\w");
                    format.indexIn(formatsList[i]);

                    QString strResolution = resolution.capturedTexts()[0];
                    QString strCode = code.capturedTexts()[0];
                    QString strFormat = format.capturedTexts()[0];
                    if (strResolution != "" && strCode != "") {
                        QVector<QString> single;
                        single.append(strResolution);
                        single.append(strCode);
                        single.append(strFormat);
                        list.append(single);
                    }
                }
    }
    return list;
}

QString Utilities::ytGetQuality() {
    return currentQualityList[win.ui->qualityComboBox->currentIndex()][1];
}

void Utilities::startConversionProcess() {
    // TODO: check if user wants conversion
    addToLog("<b>Starting conversion.</b>");

    killProcesses();

    QStringList arguments;
    arguments << "-y" << "-hide_banner" << "-i" << getCurrentRawFilename() << getCurrentFilename();

    currentConversionProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentConversionProcess->start(ffmpegBinaryName(), arguments);

    connect(currentConversionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(conversionProcess()));
    connect(currentConversionProcess, SIGNAL(finished(int)), this, SLOT(conversionComplete(int)));
}

void Utilities::downloadComplete(int code) {
    if (killed) {
        addToLog("<b>Downloading canceled.</b>");
        killed = false;
        return;
    }
    if (code != 0) {
        addToLog("<b>Download error.</b>");
        return;
    }

    addToLog("<b>Download complete</b>");
    startConversionProcess();
}

QString Utilities::ytFileName() {
    return currentID + "-" + currentQualityList[win.ui->qualityComboBox->currentIndex()][1] + "." + currentQualityList[win.ui->qualityComboBox->currentIndex()][2];
}

void Utilities::conversionProcess() {
    QString buffer = currentConversionProcess->readAllStandardOutput();
    QString duration, progress;
    QTime durationTime, progressTime;
    QRegExp time("\\d\\d\\:\\d\\d\\:\\d\\d\\.\\d\\d");
    time.indexIn(buffer);
    if (buffer.contains("Duration: ")) {
        duration = time.capturedTexts()[0];
        durationTime = QTime::fromString(duration, "hh:mm:ss.z");
        currentDuration = QTime(0,0).secsTo(durationTime);
    } else {
        if (buffer != "") {
            progress = time.capturedTexts()[0];
            progressTime = QTime::fromString(progress, "hh:mm:ss.z");
            int percent = double((QTime(0,0).secsTo(progressTime)) / double(currentDuration))*100;
            win.ui->conversionProgressBar->setValue(percent);
        }
    }
    addToLog(buffer);
}

void Utilities::resetProcesses() {
    if (utils.currentDownloadProcess->atEnd())
        utils.currentDownloadProcess->close();
    if (utils.currentConversionProcess->atEnd())
        utils.currentConversionProcess->close();
}

void Utilities::killProcesses() {
    if (utils.currentDownloadProcess->isOpen()) {
        killed = true;
        utils.currentDownloadProcess->kill();
    }
    if (utils.currentConversionProcess->isOpen()) {
        killed = true;
        utils.currentConversionProcess->kill();
    }
}

QString Utilities::ytBinaryName() {
    if (SYSTEM == "win")
        return "youtube-dl.exe";
    if (SYSTEM == "posix")
        return "./youtube-dl";
    return "";
}

QString Utilities::ffmpegBinaryName() {
    if (SYSTEM == "win")
        return "ffmpeg.exe";
    if (SYSTEM == "posix")
        return "./ffmpeg";
    return "";
}

void Utilities::conversionComplete(int code) {
    if (killed) {
        addToLog("<b>Conversion canceled.</b>");
        killed = false;
        currentDownloadProcess->deleteLater();
        currentConversionProcess->deleteLater();
        return;
    }
    if (code != 0) {
        addToLog("<b>Error on conversion. Check logs.</b>");
        return;
    }

    currentDownloadProcess->deleteLater();
    currentConversionProcess->deleteLater();

    addToLog("<b>Conversion complete.</b>");
    addToLog("Saved to: " + getCurrentFilename());

    win.lockConversionButton(false);
}

QString Utilities::getCurrentRawFilename() {
    QString path = QFileInfo(getCurrentFilename()).absolutePath();
    QString name = QFileInfo(getCurrentFilename()).baseName();
    return path + "/" + name + "." + currentQualityList[win.ui->qualityComboBox->currentIndex()][2];
}

QString Utilities::getCurrentFilename() {
    if (pathChanged)
        return currentFileName;

    return getDefaultFilename();
}

QString Utilities::getDefaultFilename() {
    return QDir().homePath() + "/" + QFileInfo(ytFileName()).baseName() + ".webm";
}
