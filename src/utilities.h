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
    void resetInterface();
    bool startProcedure();
    void addToLog(QString line);
    void setVideoDetails(QString url);

    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);
    QString currentVideoUrl;

    QVector<QVector<QString>> ytQualityList(QString url);

    QProcess *currentDownloadProcess = new QProcess;

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
