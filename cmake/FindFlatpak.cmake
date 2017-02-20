# - Try to find flatpak
# Once done, this will define
#
#  Flatpak_FOUND - system has Flatpak
#  Flatpak_INCLUDE_DIRS - the Flatpak include directories
#  Flatpak_LIBRARIES - link these to use Flatpak

find_package(PkgConfig)
pkg_check_modules(PKG_Glib QUIET glib-2.0)
pkg_check_modules(PKG_Gio QUIET gio-2.0)
pkg_check_modules(PKG_GObject QUIET gobject-2.0)
pkg_check_modules(PKG_Flatpak QUIET flatpak)

set(Flatpak_FOUND ${PKG_Flatpak_FOUND})
set(Flatpak_DEFINITIONS ${PKG_Flatpak_CFLAGS_OTHER})
set(Flatpak_VERSION ${PKG_Flatpak_VERSION})
set(Flatpak_INCLUDE_DIR ${PKG_Flatpak_INCLUDE_DIRS})

find_library(Glib_LIBRARY
    NAMES
        glib-2.0
    PATHS
        ${PKG_Glib_LIBRARY_DIRS}
)
find_library(Gio_LIBRARY
    NAMES
        gio-2.0
    PATHS
        ${PKG_Gio_LIBRARY_DIRS}
)
find_library(GObject_LIBRARY
    NAMES
        gobject-2.0
    PATHS
        ${PKG_GObject_LIBRARY_DIRS}
)

find_library(Flatpak_LIBRARY
    NAMES
        flatpak
    PATHS
        ${PKG_Flatpak_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Flatpak
    FOUND_VAR
        Flatpak_FOUND
    REQUIRED_VARS
        Flatpak_LIBRARY
        Flatpak_INCLUDE_DIR
    VERSION_VAR
        Flatpak_VERSION
)

if(Flatpak_FOUND AND NOT TARGET Flatpak::Glib)
    add_library(Flatpak::Glib UNKNOWN IMPORTED)
    set_target_properties(Flatpak::Glib PROPERTIES
	IMPORTED_LINK_DEPENDENT_LIBRARIES "${PKG_Glib_STATIC_LIBRARIES}"
        IMPORTED_LOCATION "${Glib_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PKG_Glib_CFLAGS_OTHERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${PKG_Glib_INCLUDE_DIR}"
    )
endif()

if(Flatpak_FOUND AND NOT TARGET Flatpak::Gio)
    add_library(Flatpak::Gio UNKNOWN IMPORTED)
    set_target_properties(Flatpak::Gio PROPERTIES
	IMPORTED_LINK_DEPENDENT_LIBRARIES "${PKG_Gio_STATIC_LIBRARIES}"
        IMPORTED_LOCATION "${Gio_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PKG_Gio_CFLAGS_OTHERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${PKG_Gio_INCLUDE_DIR}"
    )
endif()

if(Flatpak_FOUND AND NOT TARGET Flatpak::GObject)
    add_library(Flatpak::GObject UNKNOWN IMPORTED)
    set_target_properties(Flatpak::GObject PROPERTIES
	IMPORTED_LINK_DEPENDENT_LIBRARIES "${PKG_GObject_STATIC_LIBRARIES}"
        IMPORTED_LOCATION "${GObject_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PKG_GObject_CFLAGS_OTHERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${PKG_GObject_INCLUDE_DIR}"
    )
endif()

if(Flatpak_FOUND AND NOT TARGET Flatpak::Flatpak)
    add_library(Flatpak::Flatpak UNKNOWN IMPORTED)
    set_target_properties(Flatpak::Flatpak PROPERTIES
	IMPORTED_LINK_DEPENDENT_LIBRARIES "${PKG_Flatpak_STATIC_LIBRARIES}"
        IMPORTED_LOCATION "${Flatpak_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${Flatpak_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${Flatpak_INCLUDE_DIR}"
    )
    add_dependencies(Flatpak::Flatpak Flatpak::Glib)
    add_dependencies(Flatpak::Flatpak Flatpak::Gio)
    add_dependencies(Flatpak::Flatpak Flatpak::Gobject)
endif()

mark_as_advanced(Flatpak_LIBRARY Flatpak_INCLUDE_DIR)

# Compatibility variables
set(Flatpak_LIBRARIES ${PKG_Flatpak_LIBRARIES})
set(Flatpak_INCLUDE_DIRS ${Flatpak_INCLUDE_DIR})
set(Flatpak_VERSION_STRING ${Flatpak_VERSION})

include(FeatureSummary)
set_package_properties(Flatpak PROPERTIES
    URL "http://flatpak.org"
    DESCRIPTION "Flatpak."
)
