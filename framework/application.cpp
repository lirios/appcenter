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

#define REFINE_PROPERTY(name, value)                                                               \
    if (!value.isNull())                                                                           \
        name = value;
#define REFINE_LIST_PROPERTY(name, value) name << value;

void Application::refineFromAppstream(Appstream::Component component)
{
    REFINE_PROPERTY(m_id, component.m_id);
    REFINE_PROPERTY(m_name, component.name());
    REFINE_PROPERTY(m_summary, component.comment());
    REFINE_PROPERTY(m_icon, component.m_icon);

    foreach (Appstream::Screenshot screenshot, component.m_screenshots) {
        m_screenshots << new Screenshot(screenshot, this);
    }
}

bool Application::launch() const
{
    if (m_state != Application::Installed)
        return false;

    return m_backend->launchApplication(this);
}
