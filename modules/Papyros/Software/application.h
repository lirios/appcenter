/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QString>

#include "appstream/component.h"

class Application: public QObject
{
    Q_OBJECT

    Q_PROPERTY(State state MEMBER m_state CONSTANT)
    Q_PROPERTY(Type type MEMBER m_type CONSTANT)
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString summary MEMBER m_summary CONSTANT)
    Q_PROPERTY(QString iconName MEMBER m_iconName CONSTANT)
    Q_PROPERTY(QString latestVersion READ latestVersion CONSTANT)
    Q_PROPERTY(QString installedVersion READ installedVersion CONSTANT)

public:
    enum State {
        Installed
    };
    Q_ENUM(State)

    enum Type {
        App,
        Runtime
    };
    Q_ENUM(Type)

    Application(QObject *parent = nullptr) : QObject(parent) {}

    void refineFromAppstream(Appstream::Component component);

    virtual QString latestVersion() const = 0;
    virtual QString installedVersion() const = 0;

    State m_state;
    Type m_type;
    QString m_name;
    QString m_summary;
    QString m_iconName;
};

#endif // APPLICATION_H
