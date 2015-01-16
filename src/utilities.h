#ifndef UTILITIES_H
#define UTILITIES_H

#include <QMainWindow>
#include "mainwindow.h"

class Utilities
{
public:
    Utilities();
    Ui::MainWindow *ui;
    void setTheme();
    void setCommons();
    void setVideoDetails(QString url);
    QString ytVideoTitle(QString url);
    QString ytVideoID(QString url);

private:
    void setStylesheet();
    QString execBinary(QString bin);
    QString ytPrepareUrl(QString url);
    void setFilename(QString url);


};

extern Utilities utils;

#endif // UTILITIES_H
