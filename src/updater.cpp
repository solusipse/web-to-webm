#include "updater.h"
#include "ui_updater.h"
#include "utilities.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

Updater::Updater(QWidget *parent) : QDialog(parent), ui(new Ui::Updater) {
    ui->setupUi(this);
    start();
}

Updater::~Updater() {
    delete ui;
}

void Updater::start() {
    addToLog("Started update");
    checkForUpdates();
}

void Updater::checkForUpdates() {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl("http://solusipse.net/software/web-to-webm/version.txt")));
}

void Updater::replyFinished(QNetworkReply *reply) {
    QString newestVersion = reply->readAll();
    addToLog("Connected to update server.");
    incrementProgressBar();
    addToLog("Your web-to-webm version is: " VERSION);
    if (newestVersion == VERSION) {
        addToLog("There are no updates.");
        ui->progressBar->setValue(5);
    } else {
        addToLog("Newest version is: " + newestVersion);
        addToLog("Starting update.");
        incrementProgressBar();
    }
}

void Updater::addToLog(QString s) {
    ui->plainTextEdit->appendPlainText(s);
}

void Updater::incrementProgressBar() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}
