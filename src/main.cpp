#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtCore/QStandardPaths>
#include <QDebug>
#include <QIcon>
#include <QDir>
#include <KQuickConfig>

#define TR(x) QT_TRANSLATE_NOOP("Command line parser", QStringLiteral(x))

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("App Center");
    app.setOrganizationDomain("papyros.io");
    app.setOrganizationName("Papyros");

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    app.setDesktopFileName("io.papyros.AppCenter.desktop");
#endif

    // Set the X11 WML_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", "io.papyros.AppCenter");

    // TODO: Figure out why this is necessary
    QIcon::setThemeName("Paper");

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc://qml/main.qml")));

    return app.exec();
}
