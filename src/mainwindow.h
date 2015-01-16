#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebFrame>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_urlEdit_returnPressed();
    void on_selectSavePath_clicked();
    void on_stopConversion_clicked();
    void on_startConversion_clicked();

private:
    Ui::MainWindow *ui;
    QString prepareUrl(QString url);
};

#endif // MAINWINDOW_H
