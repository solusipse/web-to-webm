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
    void downloadProcess(QString bin);

private slots:
    void on_urlEdit_returnPressed();
    void on_selectSavePath_clicked();
    void on_stopConversion_clicked();
    void on_startConversion_clicked();

    void on_actionAbout_triggered();

    void on_qualityComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
