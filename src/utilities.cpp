#include "ui_mainwindow.h"
#include "utilities.h"
#include "window.h"

#include <QDesktopServices>
#include <QWebSettings>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QTime>

Utilities utils;

Utilities::Utilities() : settings("solusipse", "web-to-webm") {
    killed = false;
}

void Utilities::setCommons() {
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    win.ui->player->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
}

void Utilities::addToLog(QString line, bool display) {
    if (line.length() < 3)
        return;
    line.prepend("[" + QTime().currentTime().toString() + "] ");
    if (display)
        win.ui->logBox->appendHtml(line);

    line.replace("<br>", "").replace("<b>", "").replace("</b>", "");
    if (!log.contains("\n"))
        log.append(line + "\n");
    else
        log.append(line);
}

bool Utilities::checkUrl() {
    if (currentVideoUrl.isEmpty()) {
        addToLog("Add valid video first.");
        return false;
    }
    addToLog("<br><b>Started downloading video:</b><br>" + win.ui->urlEdit->text());
    return true;
}

QVector< QVector<QString> > Utilities::createQualityList(QString formats) {
    QVector< QVector<QString> > list;
    QStringList formatsList = formats.split("\n");

    /*
     * there are always clips in webm and mp4 with same resolution
     * since mp4 provides better quality and there is sometimes
     * higher quality in mp4 format available, script is excluding webm
     * at this moment
    */

    for (int i = formatsList.length()-1; i >= 0 ; i--) {
        if (!formatsList[i].contains("webm"))
            if (!formatsList[i].contains("audio only"))
                if (!formatsList[i].contains("video only")) {
                    QRegExp resolution("\\d{3,4}x\\d{3,4}");
                    resolution.indexIn(formatsList[i]);
                    QRegExp format("\\w\\w\\w");
                    format.indexIn(formatsList[i]);

                    QString strResolution = resolution.capturedTexts()[0];
                    QString strFormat = format.capturedTexts()[0];

                    if (strFormat != "" && formatsList[i].contains("unknown"))
                        strResolution = "unknown";

                    if (strResolution != "" && strFormat != "") {
                        QString strCode = formatsList[i].split(" ")[0];
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
    if (!configGetValue("youtubedl_path").isEmpty())
        return configGetValue("youtubedl_path");

    if (SYSTEM == 0)
        return "youtube-dl.exe";
    if (SYSTEM == 1)
        return QCoreApplication::applicationDirPath() + "/youtube-dl";
    return "youtube-dl";
}

QString Utilities::ffmpegBinaryName() {
    if (!configGetValue("ffmpeg_path").isEmpty())
        return configGetValue("ffmpeg_path");

    if (SYSTEM == 0)
        return "ffmpeg.exe";
    if (SYSTEM == 1)
        return QCoreApplication::applicationDirPath() + "/ffmpeg";
    return "ffmpeg";
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
    if (configGetValue("default_path").isEmpty())
        return QDir().homePath() + "/" + QFileInfo(getFileName()).baseName() + ".webm";
    return configGetValue("default_path") + "/" + QFileInfo(getFileName()).baseName() + ".webm";
}

int Utilities::getTrimmedVideoDuration() {
    QString cutFrom = win.ui->cutFromEdit->text();
    QString cutTo = win.ui->cutToEdit->text();
    int time = parseTime(cutFrom).secsTo(parseTime(cutTo));
    return time;
}

QTime Utilities::parseTime(QString s) {
    int occurences = s.count(":");

    if (occurences == 0) {
        return QTime(0,0,0).addSecs(s.toInt());
    }
    if (occurences == 1) {
        return QTime::fromString(s, "mm:ss");
    }
    if (occurences == 2) {
        return QTime::fromString(s, "hh:mm:ss");
    }

    return QTime();
}

void Utilities::startProcessQueue(QString url) {
    addToLog("Loading video details.");
    win.setLoaderHtml();

    currentVideoUrl = url;

    cmdsProcessQueue.clear();
    // Title
    cmdsProcessQueue << getBinaryName() + " -e " + url;
    // ID
    cmdsProcessQueue << getBinaryName() + " --get-id " + url;
    // Quality list
    cmdsProcessQueue << getBinaryName() + " -F " + url;

    if (processQueue != NULL) {
        processQueue->kill();
    }

    processQueue = new QProcess;
    processQueue->setProcessChannelMode(QProcess::MergedChannels);
    processQueue->start(cmdsProcessQueue[0]);

    connect(processQueue, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessQueue()));
    connect(processQueue, SIGNAL(finished(int)), this, SLOT(nextProcessQueue(int)));
    connect(processQueue, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorProcessQueue()));
}

void Utilities::errorProcessQueue() {
    addToLog("Could not open youtube-dl. Check if binary exists.");
    win.setPlayerHtml();
}

void Utilities::newVideo() {
    utils.killProcesses();
    win.setPlayerHtml();
    win.ui->titleEdit->clear();
    win.ui->urlEdit->clear();
    win.reset();
    win.lockAllControls(true);
}

void Utilities::readProcessQueue() {
    QString line = QString::fromLocal8Bit(processQueue->readAll());
    if (cmdsProcessQueue.length() >= 1) {

        if (cmdsProcessQueue.length() == 3) {
            if (line.contains("is not a valid URL."))
                currentVideoUrl = "error_url";
            else
                currentTitle = line.simplified();
        }
        if (cmdsProcessQueue.length() == 2)
            currentID = line.simplified();

        cmdsProcessQueue.removeFirst();
    }
    else {
        currentQualityList = createQualityList(line);
    }
}

void Utilities::nextProcessQueue(int c) {
    if (cmdsProcessQueue.length() <= 0 || currentVideoUrl == "error_url" || c != 0) {
        loadVideo(currentVideoUrl);
        return;
    }

    qDebug() << "DALEJ";

    processQueue->kill();
    processQueue = new QProcess;
    processQueue->start(cmdsProcessQueue[0]);

    connect(processQueue, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessQueue()));
    connect(processQueue, SIGNAL(finished(int)), this, SLOT(nextProcessQueue(int)));
}

void Utilities::loadVideo(QString url) {

    if (url == "error_url") {
        win.setPlayerHtml();
        utils.addToLog("<b>Error:</b> provided url is incorrect.");
        utils.currentVideoUrl = "";
        return;
    }

    killProcesses();
    pathChanged = false;
    win.setVideoDetails(url);
    addToLog("<b>Loaded video:</b> <br>" + win.ui->urlEdit->text());

}

void Utilities::configInit() {
    configSetValueIfBlank("remove_sound", "false");
    configSetValueIfBlank("dont_convert", "false");
    configSetValueIfBlank("remove_raw", "true");
    configSetValueIfBlank("default_path", "");
    configSetValueIfBlank("open_output", "false");
    configSetValueIfBlank("show_ytdl_log", "true");
    configSetValueIfBlank("show_ffmpeg_log", "true");
    configSetValueIfBlank("youtubedl_path", "");
    configSetValueIfBlank("ffmpeg_path", "");
    configSetValueIfBlank("ffmpeg_params", "");
    configSetValue("version", VERSION);
}

void Utilities::configSaveAll() {
    settings.setValue("remove_sound", win.ui->menuRemoveAudio->isChecked());
    settings.setValue("dont_convert", win.ui->menuDontConvert->isChecked());
    settings.setValue("remove_raw", win.ui->menuRemoveRawVideo->isChecked());
    settings.setValue("open_output", win.ui->menuShowFile->isChecked());
    settings.setValue("show_ytdl_log", win.ui->menuYoutubedlOutput->isChecked());
    settings.setValue("show_ffmpeg_log", win.ui->menuFfmpegOutput->isChecked());
}

void Utilities::configLoadAll() {
    win.ui->menuRemoveAudio->setChecked(configGetValueBool("remove_sound"));
    win.ui->menuDontConvert->setChecked(configGetValueBool("dont_convert"));
    win.ui->menuRemoveRawVideo->setChecked(configGetValueBool("remove_raw"));
    win.ui->menuShowFile->setChecked(configGetValueBool("open_output"));
    win.ui->menuYoutubedlOutput->setChecked(configGetValueBool("show_ytdl_log"));
    win.ui->menuFfmpegOutput->setChecked(configGetValueBool("show_ffmpeg_log"));
}

void Utilities::configSetValue(QString k, QString v) {
    settings.setValue(k, v);
}

void Utilities::configSetValueIfBlank(QString k, QString v) {
    if (!settings.contains(k))
        settings.setValue(k, v);
}

QString Utilities::configGetValue(QString k) {
    return settings.value(k).toString();
}

bool Utilities::configGetValueBool(QString k) {
    return settings.value(k).toBool();
}

void Utilities::removeRawVideo() {
    QFile::remove(getCurrentRawFilename());
}

void Utilities::showFileInDirectory() {
    QDesktopServices::openUrl("file:///" + QFileInfo(getCurrentFilename()).absolutePath());
}

void Utilities::updateBitrate() {
    if (!win.ui->cutFromEdit->text().trimmed().isEmpty() && !win.ui->cutToEdit->text().trimmed().isEmpty()) {
        int bitrate = win.ui->bitrateValue->text().toInt();
            bitrate = bitrate*utils.getTrimmedVideoDuration()/8;

            if (bitrate <= 0) {
                win.ui->estSize->setText("");
                return;
            }

            win.ui->estSize->setText(QString::number(float(bitrate)/1000) + " MB");
    }
}
