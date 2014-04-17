# - Find jack
# Find the native JACK includes and library
#
#  JACK_INCLUDE_DIRS - where to find jack.h, etc.
#  JACK_LIBRARIES    - List of libraries when using jack.
#  JACK_FOUND        - True if jack found.

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

IF (JACK_INCLUDE_DIR)
  # Already in cache, be silent
  SET(JACK_FIND_QUIETLY TRUE)
ENDIF (JACK_INCLUDE_DIR)

FIND_PATH(JACK_INCLUDE_DIR jack/jack.h)

IF (CMAKE_CL_64)
    SET(JACK_NAMES libjack64.lib)
ELSE()
    SET(JACK_NAMES libjack.lib)
ENDIF()

FIND_LIBRARY(JACK_LIBRARY NAMES ${JACK_NAMES})
MARK_AS_ADVANCED(JACK_LIBRARY JACK_INCLUDE_DIR)

# Per-recommendation
SET(JACK_INCLUDE_DIRS "${JACK_INCLUDE_DIR}")
SET(JACK_LIBRARIES    "${JACK_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set JACK_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JACK DEFAULT_MSG JACK_LIBRARIES JACK_INCLUDE_DIRS)
