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
    Ui::MainWindow *ui;

    // MISC methods
    void setTheme();
    void setCommons();
    void resetInterface();
    bool startProcedure();
    void addToLog(QString line);
    void setVideoDetails(QString url);
    void addQualityListToUI();
    void startConversionProcess();

    void resetProcesses();
    void killProcesses();

    // UI methods

    void lockConversionButton();
    void unlockConversionButton();
    void resetProgress();

    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);
    QString ytGetQuality();
    QString ytFileName();

    QString ytBinaryName();
    QString ffmpegBinaryName();

    // Class variables
    QString defaultFilename;
    QString currentID;
    QString currentVideoUrl;
    QString currentFilename;
    QString currentRawFilename;
    int currentDuration;

    QVector<QVector <QString> > ytQualityList(QString url);
    QVector<QVector <QString> > currentQualityList;

    QProcess *currentDownloadProcess;
    QProcess *currentConversionProcess;

private:
    void setStylesheet();
    void setFilenameUI();
    QString execBinary(QString bin, int multiline);
    QString ytPrepareUrl(QString url);
    QString getDefaultFilename(QString url);

public slots:
    void downloadProcess();
    void downloadComplete(int code);
    void conversionProcess();
    void conversionComplete(int code);

};

extern Utilities utils;

#endif // UTILITIES_H
