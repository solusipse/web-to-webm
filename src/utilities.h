#ifndef UTILITIES_H
#define UTILITIES_H

#include "mainwindow.h"
#include "downloader.h"
#include "converter.h"

#include <QMainWindow>
#include <QProcess>

#ifdef _WIN32
    #define SYSTEM 0
#else
    #define SYSTEM 1
#endif

class Utilities : public QObject
{
    Q_OBJECT

public:
    Utilities();

    QString getBinaryName();
    QString getVideoTitle(QString url);
    QString getVideoID(QString url);
    QString getVideoQuality();
    QString getFileName();
    QString ffmpegBinaryName();
    QString getCurrentRawFilename();
    QString getCurrentFilename();
    QString execBinary(QString bin, int multiline);
    QString prepareUrl(QString url);
    QString getDefaultFilename();

    bool checkUrl();
    void setCommons();
    void startConversionProcess();
    void killProcesses();
    void addToLog(QString line);
    void removeRawVideo();
    void configInit();
    void configSetValue(QString k, QString v);
    void configSetValueIfBlank(QString k, QString v);
    QString configGetValue(QString k);

    int getTrimmedVideoDuration();
    QTime parseTime(QString s);

    int currentDuration;
    bool pathChanged;
    bool loadingVideoInformations;
    bool killed;

    QString defaultFilename;
    QString currentID;
    QString currentVideoUrl;
    QString currentSavePath;
    QString currentFileName;

    QVector<QVector <QString> > ytQualityList(QString url);
    QVector<QVector <QString> > currentQualityList;

    QProcess *downloadProcess;
    QProcess *conversionProcess;

    Downloader download;
    Converter convert;

};

extern Utilities utils;

#endif // UTILITIES_H
