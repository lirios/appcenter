// SPDX-FileCopyrightText: 2016 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Liri.Core 1.0 as LiriCore
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Page {
    id: appPage

    property var app

    title: app.name

    FluidControls.SnackBar {
        id: snackBar

        duration: 5000
    }

    FluidControls.OverlayView {
        id: screenshotOverlay

        property alias source: screenshot.source

        width: Math.min(appPage.width * 0.9, screenshot.sourceSize.width)
        height: Math.min(appPage.height * 0.9, screenshot.sourceSize.height)

        Image {
            id: screenshot
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                onClicked: screenshotOverlay.close()
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: screenshot.status != Image.Ready
        }

        function show(url) {
            source = url;
            open();
        }
    }

    ScrollView {
        id: scrollView

        anchors.fill: parent
        spacing: FluidControls.Units.smallSpacing * 2

        contentWidth: appPage.width - (appPage.padding * 2)
        contentHeight: column.height

        ColumnLayout {
            id: column

            spacing: FluidControls.Units.smallSpacing * 2

            RowLayout {
                Layout.leftMargin: spacing
                Layout.rightMargin: spacing
                Layout.topMargin: spacing

                spacing: FluidControls.Units.smallSpacing * 2

                FluidControls.Icon {
                    id: icon

                    size: 64
                    name: app.iconName
                    visible: !image.visible
                }

                Image {
                    id: image

                    width: 64
                    height: width
                    source: app.iconUrl
                    visible: app.iconUrl.toString() !== ""
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignVCenter

                    FluidControls.HeadlineLabel {
                        text: app.name
                    }

                    FluidControls.SubheadingLabel {
                        text: app.summary
                        color: Material.secondaryTextColor
                    }
                }
            }

            RowLayout {
                Layout.leftMargin: spacing
                Layout.rightMargin: spacing

                spacing: FluidControls.Units.smallSpacing * 2

                visible: !transactionIndicator.visible

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    text: qsTr("Install")
                    visible: app.state === AppCenter.SoftwareResource.NotInstalledState
                    onClicked: {
                        var transaction = app.install();
                        if (!transaction) {
                            console.warn("Installation of", app.name, "has failed: null transaction");
                            return;
                        }
                        transactionIndicator.transaction = transaction;
                        transaction.run();
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    text: qsTr("Remove")
                    visible: app.state === AppCenter.SoftwareResource.InstalledState ||
                             app.state === AppCenter.SoftwareResource.UpgradableState
                    onClicked: {
                        var transaction = app.uninstall();
                        if (!transaction) {
                            console.warn("Uninstallation of", app.name, "has failed: null transaction");
                            return;
                        }
                        transactionIndicator.transaction = transaction;
                        transaction.run();
                    }

                    Material.foreground: Material.color(Material.Red, Material.Shade100)
                    Material.background: Material.color(Material.Red, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    visible: app.state === AppCenter.SoftwareResource.UpgradableState
                    text: qsTr("Update")
                    onClicked: {
                        var transaction = app.update();
                        if (!transaction) {
                            console.warn("Update of", app.name, "has failed: null transaction");
                            return;
                        }
                        transactionIndicator.transaction = transaction;
                        transaction.run();
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    visible: app.state === AppCenter.SoftwareResource.InstalledState ||
                             app.state === AppCenter.SoftwareResource.UpgradableState
                    text: qsTr("Launch")
                    onClicked: {
                        if (!app.launch())
                            console.warn("Execution of", app.name, "has failed")
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }
            }

            Transaction {
                id: transactionIndicator
            }

            Label {
                Layout.preferredWidth: appPage.width - (FluidControls.Units.smallSpacing * 2) * 2
                Layout.leftMargin: FluidControls.Units.smallSpacing * 2
                Layout.rightMargin: FluidControls.Units.smallSpacing * 2

                text: app.description
                wrapMode: Label.WordWrap
                visible: text !== ""
            }

            RowLayout {
                Layout.leftMargin: spacing
                Layout.rightMargin: spacing

                spacing: FluidControls.Units.smallSpacing * 2

                Button {
                    text: qsTr("Website")
                    visible: app.homepageUrl.toString() !== ""
                    onClicked: Qt.openUrlExternally(app.homepageUrl)
                }
            }

            ListView {
                ScrollBar.horizontal: ScrollBar {}

                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 120
                Layout.fillWidth: true
                Layout.leftMargin: FluidControls.Units.smallSpacing * 2
                Layout.rightMargin: FluidControls.Units.smallSpacing * 2

                clip: true
                orientation: ListView.Horizontal
                spacing: FluidControls.Units.smallSpacing
                visible: count > 0
                model: AppCenter.ScreenshotsModel {
                    app: appPage.app
                }
                delegate: Image {
                    id: thumbnail

                    source: model.thumbnailUrl

                    width: height * (model.thumbnailSize.width / model.thumbnailSize.height)
                    height: 120
                    visible: status !== Image.Error

                    BusyIndicator {
                        anchors.centerIn: parent
                        visible: thumbnail.status == Image.Loading
                    }

                    FluidControls.Ripple {
                        anchors.fill: parent
                        onClicked: screenshotOverlay.show(model.screenshotUrl)
                    }
                }
            }

            FluidControls.TitleLabel {
                Layout.leftMargin: FluidControls.Units.smallSpacing * 2

                text: qsTr("Details")
            }

            RowLayout {
                Layout.preferredWidth: appPage.width - ((FluidControls.Units.smallSpacing * 2) * 2)
                Layout.leftMargin: spacing
                Layout.rightMargin: spacing

                spacing: FluidControls.Units.smallSpacing * 2

                GridLayout {
                    id: kudosGrid

                    rows: 5
                    columns: 2
                    rowSpacing: FluidControls.Units.smallSpacing
                    columnSpacing: FluidControls.Units.smallSpacing

                    // Localization

                    FluidControls.Icon {
                        source: FluidControls.Utils.iconUrl("action/check_circle")
                        color: app.hasKudo(AppCenter.SoftwareResource.MyLanguageKudo)
                               ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Localized in your language")
                    }

                    // Documentation

                    FluidControls.Icon {
                        source: FluidControls.Utils.iconUrl("action/check_circle")
                        color: app.hasKudo(AppCenter.SoftwareResource.InstallsUserDocsKudo)
                               ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Documentation")
                    }

                    // Releases

                    FluidControls.Icon {
                        source: FluidControls.Utils.iconUrl("action/check_circle")
                        color: app.hasKudo(AppCenter.SoftwareResource.RecentReleaseKudo)
                               ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Release activity")
                    }

                    // System integration

                    FluidControls.Icon {
                        source: FluidControls.Utils.iconUrl("action/check_circle")
                        color: (app.hasKudo(AppCenter.SoftwareResource.SearchProviderKudo) ||
                                app.hasKudo(AppCenter.SoftwareResource.UsesNotificationsKudo) ||
                                app.hasKudo(AppCenter.SoftwareResource.HighContrastKudo))
                               ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("System integration")
                    }

                    // Sandboxed

                    FluidControls.Icon {
                        source: FluidControls.Utils.iconUrl("action/check_circle")
                        color: app.hasKudo(AppCenter.SoftwareResource.SandboxedKudo)
                               ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Sandboxed")
                    }
                }

                GridLayout {
                    id: infoGrid

                    rows: 6
                    columns: 2
                    rowSpacing: FluidControls.Units.smallSpacing
                    columnSpacing: FluidControls.Units.smallSpacing

                    // Version

                    Label {
                        text: qsTr("Version")
                    }

                    Label {
                        text: app.version
                    }

                    // Updated

                    Label {
                        text: qsTr("Updated")
                    }

                    Label {
                        text: qsTr("Unknown")
                    }

                    // Category

                    Label {
                        text: qsTr("Category")
                    }

                    Label {
                        text: app.category ? app.category : qsTr("Unknown")
                    }

                    // License

                    Label {
                        text: qsTr("License")
                    }

                    Label {
                        text: app.license ? app.license : qsTr("Unknown")
                        onLinkActivated: Qt.openUrlExternally(link)
                    }

                    // Source

                    Label {
                        text: qsTr("Source")
                    }

                    Label {
                        text: app.origin ? app.origin : qsTr("Unknown")
                    }

                    // Size

                    Label {
                        text: app.state === AppCenter.SoftwareResource.InstalledState
                              ? qsTr("Installed size") : qsTr("Download size")
                    }

                    Label {
                        text: LiriCore.Formatter.formatByteSize(app.state === AppCenter.SoftwareResource.InstalledState ? app.installedSize : app.downloadSize)
                    }
                }
            }

            FluidControls.TitleLabel {
                Layout.leftMargin: FluidControls.Units.smallSpacing * 2

                text: qsTr("Reviews")
            }

            Label {
                Layout.leftMargin: FluidControls.Units.smallSpacing * 2
                Layout.rightMargin: FluidControls.Units.smallSpacing * 2

                text: qsTr("Sorry, but reviews are not implemented yet.")
                color: Material.secondaryTextColor
            }
        }
    }
}
