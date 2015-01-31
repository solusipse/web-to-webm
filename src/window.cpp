#include "window.h"
#include "ui_mainwindow.h"
#include "utilities.h"

#include <QStyleFactory>

Window win;

Window::Window() {
}

void Window::setTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window, QColor("#333"));
    p.setColor(QPalette::Button, QColor("#333"));
    p.setColor(QPalette::Highlight, QColor("#b31217"));
    p.setColor(QPalette::ButtonText, QColor("#888"));
    p.setColor(QPalette::WindowText, QColor("#888"));
    p.setColor(QPalette::Light, QColor("#333"));
    qApp->setPalette(p);
    setStylesheet();
}

void Window::setStylesheet() {
    const char *css =
        "QPushButton:hover{background:#7d1cb4;}"
        "QWebView{background:#333;} QPlainTextEdit{background:#333;}"
        "QLineEdit{background:#888;border:none;height:35px;padding-left:10px;padding-right:10px;color:#222;}"
        "QProgressBar{background:#888; border:none;height:35px;color:#333;}"
        "QProgressBar::chunk{background:#b31217;}"
        "QPlainTextEdit{color:#888;}"
        "QComboBox{height:35px;border:none;background:#888;color:#333}"
        "QComboBox::drop-down{border:none;background:#666;}"
        "QComboBox::drop-down::pressed{background:#b31217;}"
        "QComboBox QAbstractItemView{padding:35px;background:#888;}"
        "QMenu {background:#333;}"
        "QMenu::item{}"
        "QMenu::item::selected{background:#b31217;}"
        "QPushButton{border:none;background-color:#888;padding:25px;color:#333;}"
        "QPushButton::hover{background:#b31217;}"
        "QMessageBox{background:#222;}";

    qApp->setStyleSheet(css);
}

void Window::setQualityList() {
    for (int i=0; i < utils.currentQualityList.size(); i++)
        win.ui->qualityComboBox->addItem(utils.currentQualityList[i][0]);
}

void Window::setVideoDetails(QString url) {
    // TODO: move logic to utils
    //       leave only ui-related methods here
    url = utils.ytPrepareUrl(url);

    if (url == "error") {
        win.ui->titleEdit->setText("Error: provided url is incorrect.");
        utils.addToLog("<b>Error:</b> provided url is incorrect.");
        utils.currentVideoUrl = "";
        return;
    }

    utils.loadingVideoInformations = true;
    win.reset();
    utils.pathChanged = false;

    win.ui->player->load(url);
    win.ui->titleEdit->setText(utils.ytVideoTitle(url));
    utils.currentQualityList = utils.ytQualityList(url);
    win.setQualityList();
    utils.currentFileName = utils.getDefaultFilename();
    win.setFilename();

    utils.currentVideoUrl = url;
    win.lockAllControls(false);
    utils.loadingVideoInformations = false;
    utils.addToLog("<b>Loaded video:</b><br>" + url);
}

void Window::reset() {
    win.resetProgress();
    win.ui->filenameEdit->clear();
    for (int i = win.ui->qualityComboBox->count(); i > 0; i--)
        win.ui->qualityComboBox->removeItem(0);
}

void Window::resetProgress() {
    win.ui->downloadProgressBar->setValue(0);
    win.ui->conversionProgressBar->setValue(0);
}

void Window::lockAllControls(bool status) {
    win.ui->cutFromEdit->setDisabled(status);
    win.ui->cutToEdit->setDisabled(status);
    win.ui->startConversion->setDisabled(status);
    win.ui->stopConversion->setDisabled(status);
    win.ui->selectSavePath->setDisabled(status);
}

void Window::lockConversionButton(bool v) {
    win.ui->startConversion->setDisabled(v);
}

void Window::setFilename() {
    // TODO: load default save path from config file
    win.ui->filenameEdit->setText(utils.getCurrentFilename());
}
