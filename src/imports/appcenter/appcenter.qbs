import qbs 1.0

LiriQmlPlugin {
    name: "liriappcenterplugin"
    pluginPath: "Liri/AppCenter"

    Depends { name: "LiriModels" }
    Depends { name: "LiriAppCenter" }

    files: ["*.cpp", "*.h", "qmldir"]
}
