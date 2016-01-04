#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtCore/QStandardPaths>
#include <QDebug>

#define TR(x) QT_TRANSLATE_NOOP("Command line parser", QStringLiteral(x))

int main(int argc, char *argv[])
{
        QGuiApplication app(argc, argv);
        app.setApplicationName("Papyros App Store");
        app.setOrganizationDomain("papyros.io");
        app.setOrganizationName("Papyros");

        QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                  QStringLiteral("papyros-appstore/app/main.qml"));

        if (fileName.isEmpty()) {
                qWarning() << "Failed to find the main QML file, aborting.";
                return 1;
        }

        QQmlApplicationEngine engine(fileName);


        return app.exec();
}
