// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import Liri.AppCenter as AppCenter

ColumnLayout {
    property alias iconSize: icon.iconSize

    RowLayout {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        spacing: Fluid.Units.smallSpacing

        Icon {
            id: icon

            Layout.alignment: Qt.AlignVCenter

            iconSize: 256
            iconName: app.iconName
            iconUrl: app.iconUrl
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillWidth: true

                spacing: Fluid.Units.smallSpacing

                Fluid.HeadlineLabel {
                    Layout.fillWidth: true

                    text: app.name
                    elide: Text.ElideRight
                }

                Fluid.SubheadingLabel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: app.summary
                    color: Material.secondaryTextColor
                    wrapMode: Label.WordWrap
                    elide: Text.ElideRight
                }

                Fluid.ComboBox {
                    model: AppCenter.FilteredSourcesModel {
                        sourcesModel: app ? app.sourcesModel : null
                        showDisabled: false
                    }
                    textRole: "title"
                    currentIndex: app ? app.defaultSourceIndex : 0
                    visible: count > 1
                    onCurrentIndexChanged: {
                        if (app)
                            app.defaultSource = app.sourcesModel.at(currentIndex);
                    }
                }

                // FIXME: Finish this once we have the information to show
                /*
                    Fluid.BodyLabel {
                        text: "<a href=\"www\">Developer</a>"
                        level: 2
                    }
                */

                Buttons {
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                }
            }

            Fluid.ThinDivider {
                Layout.fillWidth: true
            }

            HeaderRatings {
                Layout.fillWidth: true
            }
        }
    }
}
