import qbs 1.0

LiriQmlPlugin {
    name: "lirisoftwareplugin"
    pluginPath: "Liri/Software"

    Depends { name: "LiriCore" }
    Depends { name: "LiriSoftware" }

    cpp.defines: []

    files: ["*.cpp", "*.h", "qmldir"]
}
