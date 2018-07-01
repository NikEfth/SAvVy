# - Try to find libfreenect
# Once done, this will define
#
#  Freenect_FOUND - system has libfreenect
#  Freenect_INCLUDE_DIRS - the libfreenect include directories
#  Freenect_LIBRARIES - link these to use libfreenect

include(LibFindMacros)

# Use pkg-config to get hints about paths
# libfind_pkg_check_modules(Freenect_PKGCONF libfreenect)

# IF(NOT FREENECT_ROOT)
#   IF(EXISTS "/usr/include/libfreenect")
#     SET(FREENECT_ROOT "/usr")
#   ELSEIF(EXISTS "/usr/local/include/libfreenect")
#     SET(FREENECT_ROOT "/usr/local")
#   ELSE()
#     MESSAGE("FREENECT_ROOT not set. Continuing anyway..")
#   ENDIF()
# ENDIF()

# Include dir
find_path(Stir_INCLUDE_DIR
  PATHS ${Stir_DIR} ${Stir_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
set(Stir_LIBRARIES_DIR "${Stir_DIR}/lib")

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Stir_PROCESS_INCLUDES Stir_INCLUDE_DIR Stir_INCLUDE_DIRS)
set(Stir_PROCESS_LIBS Stir_LIBRARY Stir_LIBRARIES)
libfind_process(Stir)
 
