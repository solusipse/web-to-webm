#ifndef UTILITIES_H
#define UTILITIES_H

#include "mainwindow.h"
#include "downloader.h"
#include "converter.h"

#include <QMainWindow>
#include <QProcess>
#include <QSettings>

#ifdef _WIN32
    #define SYSTEM 0
#elif __APPLE__
    #define SYSTEM 1
#else
    #define SYSTEM 2
#endif

class Utilities : public QObject
{
    Q_OBJECT

public:
    Utilities();

    QSettings settings;

    QString getBinaryName();
    QString getVideoQuality();
    QString getFileName();
    QString ffmpegBinaryName();
    QString getCurrentRawFilename();
    QString getCurrentFilename();
    QString getDefaultFilename();

    bool checkUrl();
    void setCommons();
    void startConversionProcess();
    void killProcesses();
    void addToLog(QString line, bool display = true);
    void removeRawVideo();
    void configInit();
    void configSetValue(QString k, QString v);
    void configSetValueIfBlank(QString k, QString v);
    void showFileInDirectory();
    void configSaveAll();
    void configLoadAll();
    void loadVideo(QString url);
    void updateBitrate();
    void startProcessQueue(QString url);
    void newVideo();

    QString configGetValue(QString k);
    bool configGetValueBool(QString k);

    int getTrimmedVideoDuration();
    QTime parseTime(QString s);

    int currentDuration;
    bool pathChanged;
    bool killed;

    QString defaultFilename;
    QString currentID;
    QString currentTitle;
    QString currentVideoUrl;
    QString currentSavePath;
    QString currentFileName;
    QString log;

    QVector<QVector <QString> > createQualityList(QString formats);
    QVector<QVector <QString> > currentQualityList;

    QProcess *downloadProcess;
    QProcess *conversionProcess;
    QProcess *processQueue;

    QStringList cmdsProcessQueue;

    Downloader download;
    Converter convert;

private slots:
    void readProcessQueue();
    void nextProcessQueue(int c);
    void errorProcessQueue();

};

extern Utilities utils;

#endif // UTILITIES_H
