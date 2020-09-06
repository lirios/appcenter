// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FILTEREDSOURCESMODEL_H
#define FILTEREDSOURCESMODEL_H

#include <QSortFilterProxyModel>

#include <LiriAppCenter/SourcesModel>

class FilteredSourcesModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Liri::AppCenter::SourcesModel *sourcesModel READ sourcesModel WRITE setSourcesModel NOTIFY sourcesModelChanged)
    Q_PROPERTY(bool showDisabled READ showDisabled WRITE setShowDisabled NOTIFY showDisabledChanged)
public:
    explicit FilteredSourcesModel(QObject *parent = nullptr);

    Liri::AppCenter::SourcesModel *sourcesModel() const;
    void setSourcesModel(Liri::AppCenter::SourcesModel *sourcesModel);

    bool showDisabled() const;
    void setShowDisabled(bool show);

Q_SIGNALS:
    void sourcesModelChanged();
    void showDisabledChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    Liri::AppCenter::SourcesModel *m_sourcesModel = nullptr;
    bool m_showDisabled = true;
};

#endif // FILTEREDSOURCESMODEL_H
