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
    void setTheme();
    void setCommons();
    void addQualityListToUI();
    void startConversionProcess();
    void resetProcesses();
    void killProcesses();

    // Files manipulation
    QString getCurrentRawFilename();
    QString getCurrentFilename();

    // UI methods
    void setVideoDetails(QString url);
    void addToLog(QString line);
    void resetInterface();
    void lockConversionButton();
    void unlockConversionButton();
    void resetProgress();
    void lockAllControls(bool status);

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
    void setFilenameUI();

    bool pathChanged;
    bool loadingVideoInformations;
    bool killed;

private:
    void setStylesheet();

    QString execBinary(QString bin, int multiline);
    QString ytPrepareUrl(QString url);
    QString getDefaultFilename();


public slots:
    void downloadProcess();
    void downloadComplete(int code);
    void conversionProcess();
    void conversionComplete(int code);

};



extern Utilities utils;

#endif // UTILITIES_H
