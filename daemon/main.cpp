#include <QCoreApplication>

#include "updatenotifier.h"

#define TR(x) QT_TRANSLATE_NOOP("Command line parser", QStringLiteral(x))

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("App Center");
    app.setOrganizationDomain("papyros.io");
    app.setOrganizationName("Papyros");

    UpdateNotifier notifier;

    notifier.checkForUpdates();

    return app.exec();
}
