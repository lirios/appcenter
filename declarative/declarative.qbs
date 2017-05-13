import qbs 1.0

LiriDynamicLibrary {
    name: "Liri.Software"
    targetName: "lirisoftwareplugin"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick"] }
    Depends { name: "LiriCore" }
    Depends { name: "LiriSoftware" }

    cpp.defines: []

    files: ["*.cpp", "*.h"]

    Group {
        name: "QML Files"
        files: [
            "qmldir",
        ]
        fileTags: ["qml"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.qmlDir + "/Liri/Software"
        fileTagsFilter: ["dynamiclibrary", "qml"]
    }
}
