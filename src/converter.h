#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0);
    void start();

private:
    QString command;

public slots:
    void read();
    void complete(int code);

};

#endif // CONVERTER_H
