#include "plugin.h"

#include "software.h"
#include "source.h"
#include "application.h"
#include "appstream/screenshot.h"

void SoftwarePlugin::registerTypes(const char *uri)
{
    // @uri Papyros.Material
    Q_ASSERT(uri == QStringLiteral("Papyros.Software"));

    qmlRegisterType<Software>(uri, 0, 1, "Software");
    qmlRegisterUncreatableType<SoftwareSource>(uri, 0, 1, "SoftwareSource", "A remote for xdg-app");
    qmlRegisterUncreatableType<Application>(uri, 0, 1, "Application", "An application for xdg-app");
    qmlRegisterUncreatableType<Appstream::Screenshot>(uri, 0, 1, "Screenshot", "An screenshot for xdg-app");
}
