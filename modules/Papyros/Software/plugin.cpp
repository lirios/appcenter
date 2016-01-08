#include "plugin.h"

#include "software.h"
#include "remote.h"
#include "application.h"

void SoftwarePlugin::registerTypes(const char *uri)
{
    // @uri Papyros.Material
    Q_ASSERT(uri == QStringLiteral("Papyros.Software"));

    qmlRegisterType<Software>(uri, 0, 1, "Software");
    qmlRegisterUncreatableType<Remote>(uri, 0, 1, "Remote", "A remote for xdg-app");
    qmlRegisterUncreatableType<Application>(uri, 0, 1, "Application", "An application for xdg-app");
}
