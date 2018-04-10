import qbs 1.0

Project {
    name: "AppCenter"

    readonly property string version: "0.1.0"
    readonly property var versionParts: version.split('.').map(function(part) { return parseInt(part); })

    property bool useStaticAnalyzer: false

    condition: qbs.targetOS.contains("linux") && !qbs.targetOS.contains("android")

    minimumQbsVersion: "1.9.0"

    references: [
        "src/imports/imports.qbs",
        "src/framework/framework.qbs",
        "src/plugins/plugins.qbs",
        "src/programs.qbs",
    ]
}
