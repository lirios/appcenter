import qbs 1.0
import LiriUtils

LiriModuleProject {
    id: root

    name: "AppCenter"
    moduleName: "LiriAppCenter"
    description: "AppCenter framework"

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("AppStreamQt") }],
    })

    pkgConfigDependencies: ["Qt5Core", "LiriModels"]

    cmakeDependencies: ({ "Qt5Core": "5.6.0", "AppStreamQt": "0.11.7" })
    cmakeLinkLibraries: ["Qt5::Core", "AppStreamQt"]

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
        Depends { name: "Qt.core" }
        Depends { name: "AppStreamQt" }

        cpp.defines: [
            'LIRIAPPCENTER_VERSION="' + project.version + '"',
            "QT_BUILD_LIRIAPPCENTER_LIB"
        ]
        cpp.includePaths: base.concat([product.sourceDirectory])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt.core" }
            Depends { name: "AppStreamQt" }
        }
    }
}
