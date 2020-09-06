// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QQmlComponent>
#include <QQmlExtensionPlugin>

#include <LiriAppCenter/Rating>
#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/ResourceProxy>
#include <LiriAppCenter/Review>
#include <LiriAppCenter/ReviewsModel>
#include <LiriAppCenter/ScreenshotsModel>
#include <LiriAppCenter/SoftwareManager>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SourcesModel>
#include <LiriAppCenter/Transaction>

#include "filteredresourcesmodel.h"
#include "filteredsourcesmodel.h"
#include "filteredreviewsmodel.h"
#include "paginatemodel.h"

using namespace Liri::AppCenter;

class LiriAppCenterPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri)
    {
        // @uri Liri.AppCenter
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.AppCenter"));

        qmlRegisterType<FilteredResourcesModel>(uri, 1, 0, "FilteredResourcesModel");
        qmlRegisterType<FilteredSourcesModel>(uri, 1, 0, "FilteredSourcesModel");
        qmlRegisterType<FilteredReviewsModel>(uri, 1, 0, "FilteredReviewsModel");
        qmlRegisterType<ScreenshotsModel>(uri, 1, 0, "ScreenshotsModel");
        qmlRegisterType<SoftwareManager>(uri, 1, 0, "SoftwareManager");
        qmlRegisterType<SourcesModel>(uri, 1, 0, "SourcesModel");
        qmlRegisterType<PaginateModel>(uri, 1, 0, "PaginateModel");
        qmlRegisterUncreatableType<Rating>(uri, 1, 0, "Rating",
                                           QLatin1String("Unable to instantiate Rating"));
        qmlRegisterType<ResourcesModel>(uri, 1, 0, "ResourcesModel");
        qmlRegisterUncreatableType<ResourceProxy>(uri, 1, 0, "ResourceProxy",
                                                  QLatin1String("Unable to instantiate ResourceProxy"));
        qmlRegisterUncreatableType<Review>(uri, 1, 0, "Review",
                                           QLatin1String("Unable to instantiate Review"));
        qmlRegisterType<ReviewsModel>(uri, 1, 0, "ReviewsModel");
        qmlRegisterUncreatableType<SoftwareResource>(uri, 1, 0, "SoftwareResource", QLatin1String("Unable to instantiate SoftwareResource"));
        qmlRegisterUncreatableType<SoftwareSource>(uri, 1, 0, "SoftwareSource", QLatin1String("Unable to instantiate SoftwareSource"));
        qmlRegisterUncreatableType<Transaction>(uri, 1, 0, "Transaction",
                                                QLatin1String("Unable to instantiate Transaction"));

        qmlProtectModule(uri, 1);
    }
};

#include "plugin.moc"
