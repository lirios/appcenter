// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "flatpakbackend.h"
#include "flatpakutils.h"

QByteArray loadAppData(FlatpakInstalledRef *ref)
{
    auto *name = flatpak_installed_ref_get_appdata_name(ref);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    g_autoptr(GBytes) appstream_gz = flatpak_installed_ref_load_appdata(ref, cancellable, &error);
    if (!appstream_gz) {
        qCWarning(lcFlatpakBackend, "Failed to load appdata for \"%s\": %s",
                  name, error->message);
        return QByteArray();
    }
    g_autoptr(GZlibDecompressor) decompressor =
            g_zlib_decompressor_new(G_ZLIB_COMPRESSOR_FORMAT_GZIP);
    g_autoptr(GInputStream) stream_gz =
            g_memory_input_stream_new_from_bytes(appstream_gz);
    if (!stream_gz) {
        qCWarning(lcFlatpakBackend, "Failed to decommpress appdata for \"%s\": %s",
                  name, error->message);
        return QByteArray();
    }
    g_autoptr(GInputStream) stream_data =
            g_converter_input_stream_new(stream_gz, G_CONVERTER(decompressor));
    g_autoptr(GBytes) appstream =
            g_input_stream_read_bytes(stream_data, 0x100000, cancellable, &error);
    if (!appstream) {
        qCWarning(lcFlatpakBackend, "Failed to read appdata for \"%s\": %s",
                  name, error->message);
        return QByteArray();
    }

    gsize size = 0;
    auto *data = g_bytes_get_data(appstream, &size);
    if (data)
        return QByteArray(static_cast<const char *>(data), size);

     return QByteArray();
}
