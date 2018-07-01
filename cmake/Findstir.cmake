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
find_path(STIR_INCLUDE_DIR
  NAMES Array.h 
  CartesianCoordinate3D.h
  CartesianCoordinate2D.h
  Bin.h
  DiscretisedDensity.h
  ProjDataInfo.h
  PATHS ${STIR_DIR}/include/stir ${STIR_PKGCONF_INCLUDE_DIRS}
)

set(STIR_LIBRARIES_DIR "${STIR_DIR}/lib" CACHE PATH "description")

include_directories( ${STIR_INCLUDE_DIR})
link_directories(${STIR_LIBRARIES_DIR})

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(STIR_PROCESS_INCLUDES STIR_INCLUDE_DIR STIR_INCLUDE_DIRS)
set(STIR_PROCESS_LIBS STIR_LIBRARIES_DIR STIR_LIBRARIES)
libfind_process(Stir)
 
