// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "filteredsourcesmodel.h"

using namespace Liri::AppCenter;

FilteredSourcesModel::FilteredSourcesModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSortLocaleAware(true);
    sort(0);
}

SourcesModel *FilteredSourcesModel::sourcesModel() const
{
    return m_sourcesModel;
}

void FilteredSourcesModel::setSourcesModel(SourcesModel *sourcesModel)
{
    if (!sourcesModel || m_sourcesModel == sourcesModel)
        return;

    m_sourcesModel = sourcesModel;
    setSourceModel(m_sourcesModel);
    Q_EMIT sourcesModelChanged();
}

bool FilteredSourcesModel::showDisabled() const
{
    return m_showDisabled;
}

void FilteredSourcesModel::setShowDisabled(bool show)
{
    if (m_showDisabled == show)
        return;

    m_showDisabled = show;
    Q_EMIT showDisabledChanged();
}

bool FilteredSourcesModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!m_showDisabled) {
        QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
        return sourceModel()->data(index, SourcesModel::EnabledRole).toBool();
    }

    return true;
}

bool FilteredSourcesModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant left = sourceModel()->data(source_left, SourcesModel::TitleRole);
    QVariant right = sourceModel()->data(source_right, SourcesModel::TitleRole);
    return QString::localeAwareCompare(left.toString(), right.toString()) < 0;
}
