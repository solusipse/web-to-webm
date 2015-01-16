#ifndef UTILITIES_H
#define UTILITIES_H


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
    void setVideoDetails(QString url);
    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);
    QString currentVideoUrl;

    QProcess *currentDownloadProcess = new QProcess;

    void resetInterface();

    bool startProcedure();
    void addToLog(QString line);

private:
    void setStylesheet();
    QString execBinary(QString bin);
    QString ytPrepareUrl(QString url);
    QString getDefaultFilename(QString url);
    void setFilename(QString url);


public slots:
    void downloadProgress();


};

extern Utilities utils;

#endif // UTILITIES_H
