// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_COMPONENT_H
#define LIRI_APPSTREAM_COMPONENT_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class ComponentData;

class Component
{
public:
    enum Type {
        UnknownType,
        GenericType,
        DesktopAppType,
        ConsoleAppType,
        WebAppType,
        AddonType,
        FontType,
        CodecType,
        InputMethodType,
        FirmwareType,
        DriverType,
        LocalizationType,
        ServiceType,
        RepositoryType,
        OperatingSystemType,
        IconThemeType,
        RuntimeType
    };

    enum Merge {
        AppendMerge,
        ReplaceMerge,
        RemoveMerge
    };

    Component();

    Type type() const;
    void setType(Type type);

    int priority() const;
    void setPriority(int priority);

    Merge merge() const;
    void setMerge(Merge merge);

    QString id() const;
    void setId(const QString &id);

    QStringList packageNames();
    void addPackageName(const QString &name);

    QString sourcePackageName() const;
    void setSourcePackageName(const QString &name);

    QString name() const;
    void setName(const QString &name);

    QString projectLicense() const;
    void setProjectLicense(const QString &license);

    static Type stringToType(const QString &str);
    static QString typeToString(Type type);

    static Merge stringToMerge(const QString &str);
    static QString mergeToString(Merge merge);

private:
    QSharedDataPointer<ComponentData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_COMPONENT_H
