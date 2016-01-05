#include <QCoreApplication>
#include <QString>
#include <QProcess>
#include <QDebug>

void XAL::XAL () {
    
}

void XAL::addRemote (QString remoteName, QString url) {
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList << "add-remote" << repoName << url);
        return;
        // TODO: Error checking
}

void XAL::installApp (QString remoteName, QString appName){
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList << "install-app" << remoteName << appName);
        return;
}

void XAL::updateApp (QString appName){
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList << "update-app" << appName);
        return;
}

void XAL::installRuntime (QString remoteName, QString runtimeName){
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList << "install-runtime" <<remoteName << runtimeName);
        return;
}

QStringList XAL::listApps (){
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList << "list-apps");
        process->waitForFinished();
        QString output(process->readAllStandardOutput());
        QStringList apps = output.split("\n");
        return apps;
}
