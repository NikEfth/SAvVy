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
find_path(Qwt_INCLUDE_DIR
  PATHS ${Qwt_DIR}/src ${Qwt_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
set(Qwt_LIBRARIES_DIR "${Qwt_DIR}/lib")

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Qwt_PROCESS_INCLUDES Qwt_INCLUDE_DIR Qwt_INCLUDE_DIRS)
set(Qwt_PROCESS_LIBS Qwt_LIBRARY Qwt_LIBRARIES)
libfind_process(Qwt)
 
