// SPDX-FileCopyrightText: 2014 Morix Dev
//
// SPDX-License-Identifier: CC-BY-SA-3.0

// From: https://stackoverflow.com/questions/2690328/qt-quncompress-gzip-data/24949005#24949005

#ifndef QCOMPRESSOR_H
#define QCOMPRESSOR_H

#include <QByteArray>

#include <zlib.h>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

class QCompressor
{
public:
    static bool gzipCompress(QByteArray input, QByteArray &output, int level = -1);
    static bool gzipDecompress(QByteArray input, QByteArray &output);
};

#endif // QCOMPRESSOR_H
