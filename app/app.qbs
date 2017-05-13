import qbs 1.0

QtGuiApplication {
    name: "liri-appcenter"
    targetName: "liri-appcenter"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick", "quickcontrols2"] }

    cpp.defines: base.concat(['LIRIAPPCENTER_VERSION="' + project.version + '"'])

    files: ["*.cpp", "*.h", "*.qrc"]

    Group {
        name: "QML Files"
        files: ["qml/*.qml"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.binDir
        fileTagsFilter: product.type
    }
}
