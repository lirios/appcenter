import qbs 1.0

QtGuiApplication {
    name: "liri-appcenter"
    targetName: "liri-appcenter"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick", "quickcontrols2"] }

    Qt.core.resourcePrefix: "/"
    Qt.core.resourceSourceBase: sourceDirectory

    cpp.defines: base.concat(['LIRIAPPCENTER_VERSION="' + project.version + '"'])

    files: ["*.cpp", "*.h"]

    Group {
        name: "Resource Data"
        prefix: "qml/"
        files: ["**"]
        fileTags: ["qt.core.resource_data"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.binDir
        fileTagsFilter: product.type
    }

    Group {
        condition: qbs.targetOS.contains("linux")
        name: "Desktop File"
        files: ["io.liri.AppCenter.desktop"]
        qbs.install: true
        qbs.installDir: lirideployment.applicationsDir
    }

    Group {
        condition: qbs.targetOS.contains("linux")
        name: "AppStream Metadata"
        files: ["io.liri.AppCenter.appdata.xml"]
        qbs.install: true
        qbs.installDir: lirideployment.appDataDir
    }
}
