#include "utilities.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QWebSettings>
#include <QProcess>
#include <QDir>
#include <QTime>


Utilities utils;

Utilities::Utilities(){
}

void Utilities::setTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window, QColor("#333"));
    p.setColor(QPalette::Button, QColor("#333"));
    p.setColor(QPalette::Highlight, QColor("#b31217"));
    p.setColor(QPalette::ButtonText, QColor("#888"));
    p.setColor(QPalette::WindowText, QColor("#888"));
    p.setColor(QPalette::Light, QColor("#333"));
    qApp->setPalette(p);
    setStylesheet();
}

void Utilities::setStylesheet() {
    const char *css = "QPushButton:hover{background:#7d1cb4;}"
                      "QWebView{background:#333;} QPlainTextEdit{background:#333;}"
                      "QLineEdit{background:#888;border:none;height:35px;padding-left:10px;padding-right:10px;color:#222;}"
                      "QProgressBar{background:#888; border:none;height:35px;color:#333;}"
                      "QProgressBar::chunk{background:#b31217;}"
                      "QPlainTextEdit{color:#888;}"
                      "QComboBox{height:35px;border:none;background:#888;color:#333}"
                      "QComboBox::drop-down{border:none;background:#666;}"
                      "QComboBox::drop-down::pressed{background:#b31217;}"
                      "QComboBox QAbstractItemView{padding:35px;background:#888;}"
                      "QMenu {background:#333;}"
                      "QMenu::item{}"
                      "QMenu::item::selected{background:#b31217;}"
                      "QPushButton{border:none;background-color:#888;padding:25px;color:#333;}"
                      "QPushButton::hover{background:#b31217;}"
                      "QMessageBox{background:#222;}";

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
        addToLog("<b>Error:</b> provided url is incorrect.");
        currentVideoUrl = "";
        return;
    }

    resetInterface();

    utils.ui->player->load(url);
    utils.ui->titleEdit->setText(ytVideoTitle(url));
    currentQualityList = ytQualityList(url);
    addQualityListToUI();
    setFilenameUI();

    utils.currentVideoUrl = url;

    addToLog("<b>Loaded video:</b><br>" + url);
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

void Utilities::setFilenameUI() {
    if (!(utils.ui->filenameEdit->text().length() < 2))
        return;

    currentFilename = QDir().homePath() + "/" + QFileInfo(ytFileName()).baseName() + ".webm";
    currentRawFilename = QDir().homePath() + "/" + ytFileName();
    utils.ui->filenameEdit->setText(currentFilename);
}

void Utilities::addToLog(QString line) {
    utils.ui->logBox->appendHtml(line);
}

bool Utilities::startProcedure() {
    if (currentVideoUrl.isEmpty()) {
        addToLog("Add valid video first.");
        return false;
    }
    addToLog("<br><b>Started downloading video:</b><br>" + utils.ui->urlEdit->text());
    return true;
}

void Utilities::resetInterface() {
    resetProgress();
    utils.ui->qualityComboBox->clear();
    utils.ui->filenameEdit->clear();
}

void Utilities::downloadProcess() {
    QString buffer = utils.currentDownloadProcess->readAllStandardOutput();

    // This code if for handling progress bar
    QRegExp regexp("\\[download\\]\\s+(\\d+)\\.");
    regexp.indexIn(buffer);
    QString percent = regexp.capturedTexts()[1];
    if (percent != "")
        ui->downloadProgressBar->setValue(percent.toInt());

    if (buffer.contains("has already been downloaded"))
        ui->downloadProgressBar->setValue(100);

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

QString Utilities::ytBinaryName() {
    if (SYSTEM == "win")
        return "youtube-dl.exe";
    if (SYSTEM == "posix")
        return "./youtube-dl";
    return "";
}

void Utilities::addQualityListToUI() {
    for (int i=0; i < currentQualityList.size(); i++)
        utils.ui->qualityComboBox->addItem(currentQualityList[i][0]);
}

QString Utilities::ytGetQuality() {
    return currentQualityList[ui->qualityComboBox->currentIndex()][1];
}

void Utilities::startConversionProcess() {
    // TODO: check if user wants conversion
    addToLog("<b>Starting conversion.</b>");

    killProcesses();

    QStringList arguments;
    arguments << "-y" << "-hide_banner" << "-i" << currentRawFilename << currentFilename;

    currentConversionProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentConversionProcess->start(ffmpegBinaryName(), arguments);

    connect(currentConversionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(conversionProcess()));
    connect(currentConversionProcess, SIGNAL(finished(int)), this, SLOT(conversionComplete(int)));
}

void Utilities::downloadComplete(int code) {
    if (code != 0) {
        addToLog("<b>Download error.</b>");
        return;
    }

    addToLog("<b>Download complete</b>");
    startConversionProcess();
}

QString Utilities::ytFileName() {
    return currentID + "-" + currentQualityList[ui->qualityComboBox->currentIndex()][1] + "." + currentQualityList[ui->qualityComboBox->currentIndex()][2];
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
            ui->conversionProgressBar->setValue(percent);
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
    if (utils.currentDownloadProcess->isOpen())
        utils.currentDownloadProcess->kill();
    if (utils.currentConversionProcess->isOpen())
        utils.currentConversionProcess->kill();
}

void Utilities::lockConversionButton() {
    ui->startConversion->setDisabled(true);
}

void Utilities::unlockConversionButton() {
    ui->startConversion->setEnabled(true);
}

void Utilities::resetProgress() {
    ui->downloadProgressBar->setValue(0);
    ui->conversionProgressBar->setValue(0);
}

QString Utilities::ffmpegBinaryName() {
    if (SYSTEM == "win")
        return "ffmpeg.exe";
    if (SYSTEM == "posix")
        return "./ffmpeg";
    return "";
}

void Utilities::conversionComplete(int code) {
    if (code == 0) {
        addToLog("<b>Conversion complete.</b>");
        addToLog("Saved to: " + currentFilename);
    }
    else
        addToLog("<b>Error on conversion. Check logs.</b>");

    unlockConversionButton();
}
