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

#include "application.h"

#define REFINE_PROPERTY(name, value) if (!value.isEmpty()) name = value;
#define REFINE_LIST_PROPERTY(name, value) name << value;

void Application::refineFromAppstream(Appstream::Component component)
{
    REFINE_PROPERTY(m_name, component.name());
    REFINE_PROPERTY(m_summary, component.comment());
    REFINE_PROPERTY(m_iconName, component.m_iconName);
    REFINE_LIST_PROPERTY(m_screenshots, component.m_screenshots);
}

bool Application::launch() const
{
    if (m_state != Application::Installed)
        return false;

    return m_backend->launchApplication(this);
}