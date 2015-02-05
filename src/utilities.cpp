#include "ui_mainwindow.h"
#include "utilities.h"
#include "window.h"

#include <QWebSettings>
#include <QProcess>
#include <QDir>
#include <QTime>


Utilities utils;

Utilities::Utilities() {
    killed = false;
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
    utils.addToLog("Error on executing command: " + bin);
    return "Error on executing.";
}

QString Utilities::getVideoTitle(QString url) {
    return execBinary(getBinaryName() + " -e " + url);
}

QString Utilities::getVideoID(QString url) {
    currentID = execBinary(getBinaryName() + " --get-id " + url);
    return currentID;
}

QString Utilities::prepareUrl(QString url) {
    url = getVideoID(url);
    if (url.isEmpty())
        return "error";
    return "https://www.youtube.com/embed/" + url;
}

void Utilities::addToLog(QString line) {
    // TODO: add saving log to file option
    win.ui->logBox->appendHtml(line);
}

bool Utilities::checkUrl() {
    if (currentVideoUrl.isEmpty()) {
        addToLog("Add valid video first.");
        return false;
    }
    addToLog("<br><b>Started downloading video:</b><br>" + win.ui->urlEdit->text());
    return true;
}

QVector< QVector<QString> > Utilities::ytQualityList(QString url) {
    QVector< QVector<QString> > list;
    QString formats = execBinary(getBinaryName() + " -F " + url, 1);
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

QString Utilities::getVideoQuality() {
    return currentQualityList[win.ui->qualityComboBox->currentIndex()][1];
}


QString Utilities::getFileName() {
    return currentID + "-" + currentQualityList[win.ui->qualityComboBox->currentIndex()][1] + "." + currentQualityList[win.ui->qualityComboBox->currentIndex()][2];
}

void Utilities::killProcesses() {
    if (utils.downloadProcess != NULL) {
        killed = true;
        utils.downloadProcess->kill();
    }
    if (utils.conversionProcess != NULL) {
        killed = true;
        utils.conversionProcess->kill();
    }
}

QString Utilities::getBinaryName() {
    if (SYSTEM == 0) {
        return "youtube-dl.exe";
    }
    if (SYSTEM == 1)
        return "./youtube-dl";
    return "";
}

QString Utilities::ffmpegBinaryName() {
    if (SYSTEM == 0)
        return "ffmpeg.exe";
    if (SYSTEM == 1)
        return "./ffmpeg";
    return "";
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
    return QDir().homePath() + "/" + QFileInfo(getFileName()).baseName() + ".webm";
}

int Utilities::getTrimmedVideoDuration() {
    QString cutFrom = win.ui->cutFromEdit->text();
    QString cutTo = win.ui->cutToEdit->text();
    // TODO: add information on this to log
    return parseTime(cutFrom).secsTo(parseTime(cutTo));
}

QTime Utilities::parseTime(QString s) {
    int occurences = s.count(":");

    if (occurences == 0) {
        return QTime::QTime(0,0,0).addSecs(s.toInt());
    }
    if (occurences == 1) {
        return QTime::fromString(s, "mm:ss");
    }
    if (occurences == 2) {
        return QTime::fromString(s, "hh:mm:ss");
    }

    return QTime();
}
