#include "updater.h"
#include "ui_updater.h"
#include "utilities.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

/* TODO:
 * In the future this class needs big improvements,
 * such as checking checksum, xml parsing, adding
 * download progress bar and some archive unpacker
 * for updates. We'll probably need also some mirrors.
 */

#if _WIN32 || _WIN64
    #if _WIN64
        #define WINARCH "32"
    #else
        #define WINARCH "64"
    #endif
#endif


Updater::Updater(QWidget *parent) : QDialog(parent), ui(new Ui::Updater) {
    ui->setupUi(this);
    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{background:#888; color: #222;}");
    addToLog("After update restart is required, so finish your work before you start.");
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
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        addToLog("Could not get current version. Aborting.");
        ui->progressBar->setValue(0);
        return;
    }

    QString newestVersion = reply->readAll();
    addToLog("Connected to update server.");
    incrementProgressBar();
    addToLog("Your web-to-webm version is: " VERSION);
    if (newestVersion == VERSION) {
        addToLog("There are no updates.");
        ui->progressBar->setValue(5);
        return;
    }

    addToLog("Newest version is: " + newestVersion);
    addToLog("Downloading...");
    incrementProgressBar();

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(downloadFinished(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl("http://solusipse.net/software/web-to-webm/" + prepareBinaryName(newestVersion))));
}

void Updater::downloadFinished(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        addToLog("Could not download proper binary. Aborting.");
        ui->progressBar->setValue(0);
        return;
    }
    addToLog("Download complete.");
    incrementProgressBar();
    QFile f(QCoreApplication::applicationDirPath() + "/update");
    if (f.open(QIODevice::WriteOnly)) {
        f.write(reply->readAll());
        f.close();
    }
    addToLog("Saved to file.");
    addToLog("Application will be restarted now.");

    QString pid = QString::number(QCoreApplication::applicationPid());

    QProcess *proc = new QProcess(this);
    proc->startDetached(QCoreApplication::applicationDirPath() + "/renamer " + pid);

    qApp->exit();
}

void Updater::addToLog(QString s) {
    ui->plainTextEdit->appendPlainText(s);
}

void Updater::incrementProgressBar() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

QString Updater::prepareBinaryName(QString v) {
    if (SYSTEM == 0)
        return "win-" + QString(WINARCH) + "-web-to-webm-" + v;
    if (SYSTEM == 1)
        return "mac-web-to-webm-" + v;
    if (SYSTEM == 2)
        return "linux-web-to-webm-" + v;
}

void Updater::on_buttonStart_clicked()
{
    utils.killProcesses();
    start();
}
