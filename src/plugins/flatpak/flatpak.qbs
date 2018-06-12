import qbs 1.0
import qbs.FileInfo

LiriDynamicLibrary {
    name: "FlatpakBackend"
    targetName: "flatpak"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "concurrent", "network"] }
    Depends { name: "flatpak" }
    Depends { name: "AppStreamQt" }
    Depends { name: "LiriAppCenter" }

    condition: {
        if (!flatpak.found) {
            console.error("flatpak is required to build " + targetName);
            return false;
        }

        if (!AppStreamQt.found) {
            console.error("AppStreamQt is required to build " + targetName);
            return false;
        }

        return true;
    }

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: FileInfo.joinPaths(lirideployment.pluginsDir, "liri", "appcenter")
        fileTagsFilter: product.type
    }

    Group {
        condition: qbs.targetOS.contains("linux")
        name: "Desktop File"
        files: ["io.liri.AppCenter.Flatpak.desktop"]
        qbs.install: true
        qbs.installDir: lirideployment.applicationsDir
    }
}
