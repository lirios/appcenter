#include "plugin.h"

#include "xdg-app-plugin.h"

void SoftwarePlugin::registerTypes(const char *uri)
{
    // @uri Papyros.Material
    Q_ASSERT(uri == QStringLiteral("Papyros.Software"));

    qmlRegisterType<XdgApp>(uri, 0, 1, "XdgApp");
    qmlRegisterUncreatableType<Remote>(uri, 0, 1, "Remote", "A remote for xdg-app");
}
