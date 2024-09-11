// SPDX-FileCopyrightText: 2016 Michael Spencer <sonrisesoftware@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtCore/QStandardPaths>
#include <QDebug>
#include <QIcon>
#include <QDir>
#include <QTranslator>
#include <QtQuickControls2/QQuickStyle>

using namespace Qt::StringLiterals;

#if 0
static void loadQtTranslations()
{
#ifndef QT_NO_TRANSLATION
    QString locale = QLocale::system().name();

    // Load Qt translations
    QTranslator *qtTranslator = new QTranslator(QCoreApplication::instance());
    if (qtTranslator->load(QStringLiteral("qt_%1").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qApp->installTranslator(qtTranslator);
    } else {
        delete qtTranslator;
    }
#endif
}

static void loadAppTranslations()
{
#ifndef QT_NO_TRANSLATION
    QString locale = QLocale::system().name();

    // Find the translations directory
    const QString path = QLatin1String("liri-appcenter/translations");
    const QString translationsDir =
        QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                               path,
                               QStandardPaths::LocateDirectory);

    // Load shell translations
    QTranslator *appTranslator = new QTranslator(QCoreApplication::instance());
    if (appTranslator->load(QStringLiteral("%1/appcenter_%3").arg(translationsDir, locale))) {
        QCoreApplication::installTranslator(appTranslator);
    } else if (locale == QLatin1String("C") ||
                locale.startsWith(QLatin1String("en"))) {
        // English is the default, it's translated anyway
        delete appTranslator;
    }
#endif
}
#endif

int main(int argc, char *argv[])
{
    // Set the X11 WML_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", QByteArrayLiteral("io.liri.AppCenter"));

    QGuiApplication app(argc, argv);
    app.setApplicationName("App Center"_L1);
    app.setOrganizationDomain("liri.io"_L1);
    app.setOrganizationName("Liri"_L1);
    app.setDesktopFileName("io.liri.AppCenter"_L1);

    QQuickStyle::setStyle("Material"_L1);

#if 0
    // Load translations
    loadQtTranslations();
    loadAppTranslations();
#endif

    QQmlApplicationEngine engine;
    QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
            []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("io.liri.AppCenter", "Main");

    return app.exec();
}
