import qbs 1.0
import "../qbs/shared/imports/LiriUtils.js" as LiriUtils

LiriModuleProject {
    id: root

    name: "Software"
    moduleName: "LiriSoftware"
    description: "AppCenter framework"

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") }, { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.xml") }, { name: LiriUtils.quote("Qt.concurrent") },
                  { name: LiriUtils.quote("flatpak") }, { name: LiriUtils.quote("KF5.Archive") }],
    })

    pkgConfigDependencies: ["Qt5Core", "LiriModels"]

    cmakeDependencies: ({ "Qt5Core": "5.6.0", "LiriCore": "", "KF5Archive": "5.23.0" })
    cmakeLinkLibraries: ["Qt5::Core", "LiriModels::LiriModels", "KF5::Archive"]

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName

        Group {
            name: "Headers"
            files: "*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriModule {
        name: root.moduleName
        targetName: root.targetName
        version: "0.0.0"

        Depends { name: root.headersName }
        Depends { name: "Qt"; submodules: ["core", "gui", "xml", "concurrent"] }
        Depends { name: "LiriModels" }
        Depends { name: "flatpak" }
        Depends { name: "KF5.Archive" }

        condition: {
            if (!KF5.Archive.found) {
                console.error("KF5Archive is required to build " + targetName);
                return false;
            }

            if (!flatpak.found) {
                console.error("flatpak is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: [
            'LIRISOFTWARE_VERSION="' + project.version + '"',
            "QT_BUILD_LIRISOFTWARE_LIB"
        ]
        cpp.includePaths: base.concat([product.sourceDirectory])

        files: ["*.cpp", "*.h", "flatpak/*.cpp", "flatpak/*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt"; submodules: ["core", "gui", "xml", "concurrent"] }
            Depends { name: "LiriModels" }
            Depends { name: "KF5.Archive" }
        }
    }
}
