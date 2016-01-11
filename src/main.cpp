#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtCore/QStandardPaths>
#include <QDebug>
#include <QIcon>
#include <QDir>

#define TR(x) QT_TRANSLATE_NOOP("Command line parser", QStringLiteral(x))

int main(int argc, char *argv[])
{
        QGuiApplication app(argc, argv);
        app.setApplicationName("App Center");
        app.setOrganizationDomain("papyros.io");
        app.setOrganizationName("Papyros");

        #if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
            app.setDesktopFileName("io.papyros.AppCenter");
        #endif

        // Set the X11 WML_CLASS so X11 desktops can find the desktop file
        qputenv("RESOURCE_NAME", "io.papyros.AppCenter");

        // TODO: Figure out why this is necessary
        QIcon::setThemeName("Paper");

        QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                  QStringLiteral("papyros-appcenter/app/main.qml"));

        if (fileName.isEmpty()) {
                qWarning() << "Failed to find the main QML file, aborting.";
                return 1;
        }

        QQmlApplicationEngine engine(fileName);

        return app.exec();
}
