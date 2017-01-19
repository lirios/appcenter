# - Try to find flatpak
# Once done, this will define
#
#  Flatpak_FOUND - system has Flatpak
#  Flatpak_INCLUDE_DIRS - the Flatpak include directories
#  Flatpak_LIBRARIES - link these to use Flatpak

include(LibFindMacros)

# Dependencies
libfind_package(Flatpak Glib)
libfind_package(Flatpak GObject)
libfind_package(Flatpak GIO)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(Flatpak_PKGCONF flatpak)

# Include dir
find_path(Flatpak_INCLUDE_DIR
  NAMES flatpak.h
  PATHS ${Flatpak_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(Flatpak_LIBRARY
  NAMES flatpak
  PATHS ${Flatpak_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Flatpak_PROCESS_INCLUDES Flatpak_INCLUDE_DIR Glib_INCLUDE_DIR GObject_INCLUDE_DIR GIO_INCLUDE_DIR)
set(Flatpak_PROCESS_LIBS Flatpak_LIBRARY Glib_LIBRARY GObject_LIBRARY GIO_LIBRARY)
libfind_process(Flatpak)
