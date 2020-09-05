// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "translation.h"

namespace Liri {

namespace AppStream {

/*
 * TranslationData
 */

class TranslationData : public QSharedData
{
public:
    TranslationData()
    {
    }
};

/*
 * Translation
 */

Translation::Translation()
    : d(new TranslationData())
{
}

} // namespace AppStream

} // namespace Liri
