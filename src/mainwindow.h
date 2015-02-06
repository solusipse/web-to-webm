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
    void on_startConversion_clicked();
    void on_menuAbout_triggered();
    void on_qualityComboBox_currentIndexChanged(int index);
    void on_menuWebsite_triggered();
    void on_menuDefaultDownloadPath_triggered();
    void on_menuExit_triggered();
    void on_menuClearLog_triggered();
    void on_menuShowLog_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
