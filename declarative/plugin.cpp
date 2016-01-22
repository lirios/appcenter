#include "plugin.h"

#include "softwaremanager.h"
#include "source.h"
#include "application.h"
#include "screenshot.h"

void SoftwarePlugin::registerTypes(const char *uri)
{
    // @uri Papyros.Material
    Q_ASSERT(uri == QStringLiteral("Papyros.Software"));

    qmlRegisterType<SoftwareManager>(uri, 0, 1, "SoftwareManager");
    qmlRegisterUncreatableType<SoftwareSource>(uri, 0, 1, "SoftwareSource", "A remote for xdg-app");
    qmlRegisterUncreatableType<Application>(uri, 0, 1, "Application", "An application for xdg-app");
    qmlRegisterUncreatableType<Screenshot>(uri, 0, 1, "Screenshot", "An screenshot for xdg-app");
}
