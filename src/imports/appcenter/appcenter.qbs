import qbs 1.0

LiriQmlPlugin {
    name: "liriappcenterplugin"
    pluginPath: "Liri/AppCenter"

    Depends { name: "LiriCore" }
    Depends { name: "LiriSoftware" }

    cpp.defines: []

    files: [
        "plugin.cpp",
        "qmldir"
    ]
}
