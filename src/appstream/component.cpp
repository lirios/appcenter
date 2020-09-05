// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "component.h"

namespace Liri {

namespace AppStream {

/*
 * ComponentData
 */

class ComponentData : public QSharedData
{
public:
    ComponentData()
    {
    }

    Component::Type type = Component::UnknownType;
    int priority = 0;
    Component::Merge merge = Component::AppendMerge;
    QString id;
    QStringList packageNames;
    QString sourcePackageName;
    QString name;
    QString license;
};

/*
 * Component
 */

typedef QHash<Component::Type, QString> TypeMap;
Q_GLOBAL_STATIC_WITH_ARGS(
        TypeMap, typeMap,
        ({
             { Component::UnknownType, QLatin1String("unknown") },
             { Component::GenericType, QLatin1String("generic") },
             { Component::DesktopAppType, QLatin1String("desktop-application") },
             { Component::ConsoleAppType, QLatin1String("console-application") },
             { Component::WebAppType, QLatin1String("web-application") },
             { Component::AddonType, QLatin1String("addon") },
             { Component::FontType, QLatin1String("font") },
             { Component::CodecType, QLatin1String("codec") },
             { Component::InputMethodType, QLatin1String("inputmethod") },
             { Component::FirmwareType, QLatin1String("firmware") },
             { Component::DriverType, QLatin1String("driver") },
             { Component::LocalizationType, QLatin1String("localization") },
             { Component::ServiceType, QLatin1String("service") },
             { Component::RepositoryType, QLatin1String("repository") },
             { Component::OperatingSystemType, QLatin1String("operating-system") },
             { Component::IconThemeType, QLatin1String("icon-theme") },
             { Component::RuntimeType, QLatin1String("runtime") }
         }));

typedef QHash<Component::Merge, QString> MergeMap;
Q_GLOBAL_STATIC_WITH_ARGS(
        MergeMap, mergeMap,
        ({
             { Component::AppendMerge, QLatin1String("append") },
             { Component::ReplaceMerge, QLatin1String("replace") },
             { Component::RemoveMerge, QLatin1String("remove-component") }
         }));

Component::Component()
    : d(new ComponentData())
{
}

Component::Type Component::type() const
{
    return d->type;
}

void Component::setType(Component::Type type)
{
    d->type = type;
}

int Component::priority() const
{
    return d->type;
}

void Component::setPriority(int priority)
{
    d->priority = priority;
}

Component::Merge Component::merge() const
{
    return d->merge;
}

void Component::setMerge(Component::Merge merge)
{
    d->merge = merge;
}

QString Component::id() const
{
    return d->id;
}

void Component::setId(const QString &id)
{
    d->id = id;
}

QStringList Component::packageNames()
{
    return d->packageNames;
}

void Component::addPackageName(const QString &name)
{
    d->packageNames.append(name);
}

QString Component::sourcePackageName() const
{
    return d->sourcePackageName;
}

void Component::setSourcePackageName(const QString &name)
{
    d->sourcePackageName = name;
}

QString Component::name() const
{
    return d->name;
}

void Component::setName(const QString &name)
{
    d->name = name;
}

QString Component::projectLicense() const
{
    return d->license;
}

void Component::setProjectLicense(const QString &license)
{
    d->license = license;
}

Component::Type Component::stringToType(const QString &str)
{
    return typeMap->key(str);
}

QString Component::typeToString(Component::Type type)
{
    return typeMap->value(type);
}

Component::Merge Component::stringToMerge(const QString &str)
{
    return mergeMap->key(str);
}

QString Component::mergeToString(Component::Merge merge)
{
    return mergeMap->value(merge);
}

} // namespace AppStream

} // namespace Liri
