/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2013-2015 Richard Hughes <richard@hughsie.com>
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

#include "component.h"

#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QDir>
#include <QDebug>

using namespace Appstream;


Url::Url(QString url, Type type)
        : m_url(url), m_type(type)
{
    // Nothing needed here
}

Url::Url(QString url, QString type)
        : m_url(url)
{
    if (type == "homepage")
        m_type = Url::Homepage;
    else if (type == "bugtracker")
        m_type = Url::BugTracker;
    else if (type == "faq")
        m_type = Url::FAQ;
    else if (type == "donation")
        m_type = Url::Donation;
    else if (type == "help")
        m_type = Url::Help;
    else if (type == "missing")
        m_type = Url::Missing;
    else
       m_type = Url::Unknown;
}

bool Url::operator==(const Url &other) const
{
    return m_type == other.m_type && m_url == other.m_url;
}
