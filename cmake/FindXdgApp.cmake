# - Try to find xdg-app
# Once done, this will define
#
#  XdgApp_FOUND - system has XdgApp
#  XdgApp_INCLUDE_DIRS - the XdgApp include directories
#  XdgApp_LIBRARIES - link these to use XdgApp

include(LibFindMacros)

# Dependencies
libfind_package(XdgApp Glib)
libfind_package(XdgApp GObject)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(XdgApp_PKGCONF xdg-app)

# Include dir
find_path(XdgApp_INCLUDE_DIR
  NAMES xdg-app.h
  PATHS ${XdgApp_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(XdgApp_LIBRARY
  NAMES xdg-app
  PATHS ${XdgApp_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(XdgApp_PROCESS_INCLUDES XdgApp_INCLUDE_DIR Glib_INCLUDE_DIRS)
set(XdgApp_PROCESS_LIBS XdgApp_LIBRARY Glib_LIBRARIES)
libfind_process(XdgApp)
