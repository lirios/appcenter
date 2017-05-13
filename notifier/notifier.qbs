import qbs 1.0

CppApplication {
    name: "liri-update-notifier"
    targetName: "liri-update-notifier"

    Depends { name: "lirideployment" }
    Depends { name: "Qt.core" }
    Depends { name: "LiriNotifications" }
    Depends { name: "LiriSoftware" }

    cpp.defines: base.concat(['APPCENTER_VERSION="' + project.version + '"'])

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.binDir
        fileTagsFilter: product.type
    }
}
