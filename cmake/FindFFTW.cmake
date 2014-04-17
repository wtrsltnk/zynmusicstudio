# - Find fftw
# Find the native FFTW includes and library
#
#  FFTW_INCLUDE_DIRS - where to find fftw3.h, etc.
#  FFTW_LIBRARIES    - List of libraries when using fftw.
#  FFTW_FOUND        - True if fftw found.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

IF (FFTW_INCLUDE_DIR)
  # Already in cache, be silent
  SET(FFTW_FIND_QUIETLY TRUE)
ENDIF (FFTW_INCLUDE_DIR)

FIND_PATH(FFTW_INCLUDE_DIR fftw3.h)

SET(FFTW_NAMES fftw3 libfftw fftw.lib fftwd.lib libfftw3-3.lib)
FIND_LIBRARY(FFTW_LIBRARY NAMES ${FFTW_NAMES} )
MARK_AS_ADVANCED( FFTW_LIBRARY FFTW_INCLUDE_DIR )

# Per-recommendation
SET(FFTW_INCLUDE_DIRS "${FFTW_INCLUDE_DIR}")
SET(FFTW_LIBRARIES    "${FFTW_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set FFTW_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FFTW DEFAULT_MSG FFTW_LIBRARIES FFTW_INCLUDE_DIRS)
