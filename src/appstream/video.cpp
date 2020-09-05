// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "video.h"

namespace Liri {

namespace AppStream {

/*
 * VideoData
 */

class VideoData : public QSharedData
{
public:
    VideoData()
    {
    }
};

/*
 * Video
 */

Video::Video()
    : d(new VideoData())
{
}

} // namespace AppStream

} // namespace Liri
