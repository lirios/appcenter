import qbs 1.0

LiriModule {
    name: "LiriSoftware"
    targetName: "LiriSoftware"
    version: "0.0.0"

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

    create_headers.headersMap: ({
        "application.h": "Application",
        "screenshot.h": "Screenshot",
        "softwaremanager.h": "SoftwareManager",
        "source.h": "Source",
    })

    create_pkgconfig.name: "Liri Software"
    create_pkgconfig.description: "AppCenter framework"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "LiriModels"]

    create_cmake.version: project.version
    create_cmake.dependencies: ({
        "Qt5Core": "5.6.0",
        "LiriCore": "",
        "KF5Archive": "5.23.0",
    })
    create_cmake.linkLibraries: ["Qt5::Core", "LiriModels::LiriModels", "KF5::Archive"]

    files: ["*.cpp", "appstream/*.cpp", "appstream/*.h", "flatpak/*.cpp", "flatpak/*.h"]

    Group {
        name: "Headers"
        files: ["*.h"]
        excludeFiles: ["*_p.h"]
        fileTags: ["public_headers"]
    }

    Group {
        name: "Private Headers"
        files: ["*_p.h"]
        fileTags: ["private_headers"]
    }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "gui", "xml", "concurrent"] }
        Depends { name: "LiriModels" }
        Depends { name: "KF5.Archive" }

        cpp.includePaths: base.concat([product.sourceDirectory, product.generatedHeadersDir])
    }
}
