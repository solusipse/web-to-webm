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

    void setTheme();
    void setCommons();
    void resetInterface();
    bool startProcedure();
    void addToLog(QString line);
    void setVideoDetails(QString url);
    void addQualityListToUI();

    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);
    QString currentVideoUrl;
    QString ytBinaryName();
    QString ytGetQuality();

    QVector<QVector <QString> > ytQualityList(QString url);
    QVector<QVector <QString> > currentQualityList;

    QProcess *currentDownloadProcess;

private:
    void setStylesheet();
    void setFilename(QString url);
    QString execBinary(QString bin, int multiline);
    QString ytPrepareUrl(QString url);
    QString getDefaultFilename(QString url);

public slots:
    void downloadProgress();

};

extern Utilities utils;

#endif // UTILITIES_H
