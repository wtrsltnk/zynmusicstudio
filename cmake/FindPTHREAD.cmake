# - Find PTHREAD
# Find the native PTHREAD includes and library
#
#  PTHREAD_INCLUDE_DIRS - where to find pthread.h, etc.
#  PTHREAD_LIBRARIES    - List of libraries when using pthread.
#  PTHREAD_FOUND        - True if pthread found.

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

IF (PTHREAD_INCLUDE_DIR)
  # Already in cache, be silent
  SET(PTHREAD_FIND_QUIETLY TRUE)
ENDIF (PTHREAD_INCLUDE_DIR)

FIND_PATH(PTHREAD_INCLUDE_DIR pthread.h)

SET(PTHREAD_NAMES pthread pthreadVC2)
FIND_LIBRARY(PTHREAD_LIBRARY NAMES ${PTHREAD_NAMES} )
MARK_AS_ADVANCED( PTHREAD_LIBRARY PTHREAD_INCLUDE_DIR )

# Per-recommendation
SET(PTHREAD_INCLUDE_DIRS "${PTHREAD_INCLUDE_DIR}")
SET(PTHREAD_LIBRARIES    "${PTHREAD_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set PTHREAD_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PTHREAD DEFAULT_MSG PTHREAD_LIBRARIES PTHREAD_INCLUDE_DIRS)
