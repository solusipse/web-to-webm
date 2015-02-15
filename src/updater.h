#ifndef UPDATER_H
#define UPDATER_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class Updater;
}

class Updater : public QDialog
{
    Q_OBJECT

public:
    explicit Updater(QWidget *parent = 0);
    ~Updater();

private:
    Ui::Updater *ui;
    void checkForUpdates();
    void addToLog(QString s);
    void start();
    void incrementProgressBar();

public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // UPDATER_H
