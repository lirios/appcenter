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
#include <AppstreamQt/database.h>
#include <AppstreamQt/component.h>

#include "base.h"

class Application: public QObject
{
    Q_OBJECT

    Q_PROPERTY(State state MEMBER m_state CONSTANT)
    Q_PROPERTY(Type type MEMBER m_type CONSTANT)
    Q_PROPERTY(QString branch MEMBER m_branch CONSTANT)
    Q_PROPERTY(QString origin MEMBER m_origin CONSTANT)
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString summary MEMBER m_summary CONSTANT)
    Q_PROPERTY(QString arch MEMBER m_arch CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)

public:
    enum State {
        Installed
    };

    enum Type {
        App,
        Runtime
    };

    Application(XdgAppInstalledRef *app_ref, State state, QObject *parent = nullptr)
        : QObject(parent)
    {
        m_state = state;
        m_branch = xdg_app_ref_get_branch(XDG_APP_REF(app_ref));
        m_origin = xdg_app_installed_ref_get_origin(app_ref);
        m_name = xdg_app_ref_get_name(XDG_APP_REF(app_ref));
        m_arch = xdg_app_ref_get_arch(XDG_APP_REF(app_ref));

        QString desktopId;

    	switch (xdg_app_ref_get_kind(XDG_APP_REF(app_ref))) {
    	case XDG_APP_REF_KIND_APP:
            m_type = Application::App;

            desktopId = m_name + ".desktop";
    		break;
    	case XDG_APP_REF_KIND_RUNTIME:
            m_type = Application::Runtime;
            m_summary = "Framework for applications";

            desktopId = m_name + ".runtime";
    		break;
    	default:
    		// TODO: Handle errors here!
            break;
    	}

        QString deployDir = xdg_app_installed_ref_get_deploy_dir(app_ref);
        QString desktopPath = deployDir + "/files/share/applications";
        QString appdataPath = deployDir + "/files/share/appdata";

        m_component = findComponent(QStringList() << desktopPath << appdataPath,
            desktopId);
    }

    QString iconName() const
    {
        return m_type == Application::Runtime ? "application-x-executable"
                                              : m_component.icon();
    }

    QUrl iconUrl(const QSize& size) const
    {
        return m_component.iconUrl(size);
    }

    static Appstream::Component findComponent(QStringList paths, QString id)
    {
        Q_FOREACH(QString path, paths) {
            Appstream::Database database(path);
            if (!database.open())
                return Appstream::Component();

            Appstream::Component component = database.componentById(id);

            if (component.isValid())
                return component;
        }

        return Appstream::Component();
    }

    Appstream::Component m_component;
    State m_state;
    Type m_type;
    QString m_branch;
    QString m_origin;
    QString m_name;
    QString m_summary;
    QString m_iconName;
    QString m_arch;
};

#endif // APPLICATION_H
