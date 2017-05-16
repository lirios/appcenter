import qbs 1.0

Project {
    name: "Liri AppCenter"

    readonly property string version: "0.1.0"

    minimumQbsVersion: "1.6"

    qbsSearchPaths: "qbs/shared"

    references: [
        "app/app.qbs",
        "declarative/declarative.qbs",
        "framework/framework.qbs",
        "notifier/notifier.qbs",
    ]
}
