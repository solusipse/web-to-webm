#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef _WIN32
    #define SYSTEM "win"
#else
    #define SYSTEM "posix"
#endif


#include <QMainWindow>
#include <QProcess>
#include "mainwindow.h"


class Utilities : public QObject
{
    Q_OBJECT

public:
    Utilities();

    // Download-related methods
    bool startProcedure();
    QString ytBinaryName();
    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);
    QString ytGetQuality();
    QString ytFileName();

    // Conversion-related methods
    QString ffmpegBinaryName();

    // MISC methods
    void setCommons();

    void startConversionProcess();
    void resetProcesses();
    void killProcesses();

    // Files manipulation
    QString getCurrentRawFilename();
    QString getCurrentFilename();

    // UI methods
    void addToLog(QString line);

    // Class variables
    QString defaultFilename;
    QString currentID;
    QString currentVideoUrl;
    QString currentSavePath;
    QString currentFileName;
    int currentDuration;

    QVector<QVector <QString> > ytQualityList(QString url);
    QVector<QVector <QString> > currentQualityList;

    // Processes
    QProcess *currentDownloadProcess;
    QProcess *currentConversionProcess;

    bool pathChanged;
    bool loadingVideoInformations;
    bool killed;

    QString execBinary(QString bin, int multiline);
    QString ytPrepareUrl(QString url);
    QString getDefaultFilename();

private:
    // TODO



public slots:
    void downloadProcess();
    void downloadComplete(int code);
    void conversionProcess();
    void conversionComplete(int code);

};



extern Utilities utils;

#endif // UTILITIES_H
