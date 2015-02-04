#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QProcess>

class Downloader : public QObject {
    Q_OBJECT

public:
    explicit Downloader(QObject *parent = 0);
    void start(QString c, QStringList l);

private:
    QString command;

public slots:
    void read();
    void complete(int code);

};


#endif // DOWNLOADER_H
