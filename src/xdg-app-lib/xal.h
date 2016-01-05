#include <QCoreApplication>
#include <QString>
#include <QProcess>
#include <QDebug>

class XAL
{
public:
        void addRemote(QString repoName, QString url) const;
        void installApp(QString remoteName, QString appName) const;
        void updateApp (QString appName) const;
        void installRuntime (QString remoteName, QString runtimeName) const;
        QStringList listApps () const;
}
