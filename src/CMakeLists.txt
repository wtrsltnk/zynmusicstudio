message(STATUS "Checking Include Path" $ENV{CMAKE_INCLUDE_PATH} ${CMAKE_INCLUDE_PATH})
message(STATUS "Checking Library Path" $ENV{CMAKE_LIBRARY_PATH} ${CMAKE_LIBRARY_PATH})
message(STATUS "Building on a '${CMAKE_SYSTEM_NAME} v${CMAKE_SYSTEM_VERSION}' System and an '${CMAKE_SYSTEM_PROCESSOR}' CPU")

find_package(FFTW REQUIRED)
find_package(MXML REQUIRED)
find_package(PTHREAD REQUIRED)

link_directories(
    ${FFTW_LIBRARY_DIRS}
    ${MXML_LIBRARY_DIRS}
    ${PTHREAD_LIBRARY_DIRS}
    )

include_directories(
    ${FFTW_INCLUDE_DIR}
    ${MXML_INCLUDE_DIR}
    ${PTHREAD_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

add_definitions(-DVERSION="${VERSION}")
add_definitions(-DPIXMAP_PATH="${CMAKE_INSTALL_PREFIX}/share/zynaddsubfx/pixmaps/")
add_definitions(-DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    if(CMAKE_CL_64)
        add_definitions(-D_AMD64_)
    endif(CMAKE_CL_64)
    add_definitions(-D__WINDOWS_MM__)
    set(OS_LIBRARIES "winmm.lib")
else(MSVC)
    option(BuildForAMD_X86_64 "Build for AMD x86_64 system" OFF)
    option(BuildForCore2_X86_64 "Build for Intel Core2 x86_64 system" OFF)
    option(BuildForDebug "Include gdb debugging support" OFF)

    set(CMAKE_BUILD_TYPE "Release")

    set(BuildOptions_x86_64AMD
        "-O3 -march=athlon64 -m64 -Wall -ffast-math -fno-finite-math-only -fomit-frame-pointer"
        CACHE STRING "X86_64 compiler options"
    )

    set(BuildOptions_X86_64Core2
        "-O3 -march=core2 -m64 -Wall -ffast-math -fno-finite-math-only -fomit-frame-pointer"
        CACHE STRING "X86_64 compiler options"
    )

    set(BuildOptionsBasic
        "-O3 -msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer"
        CACHE STRING "basic X86 complier options"
    )

    set(BuildOptionsDebug
        "-O0 -g3 -ggdb -Wall -Wpointer-arith" CACHE STRING "Debug build flags"
    )

    add_definitions(
         -g #TODO #todo put in a better location
         -Wall
         -Wextra
         )
    if(NOT AVOID_ASM)
        message(STATUS "Compiling with x86 opcode support")
        add_definitions(-DASM_F2I_YES)
    endif()

    if(BuildForDebug)
        set(CMAKE_BUILD_TYPE "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG ${BuildOptionsDebug})
        message(STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    else(BuildForDebug)
        set(CMAKE_BUILD_TYPE "Release")
        if(BuildForAMD_X86_64)
            set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptions_x86_64AMD})
        else(BuildForAMD_X86_64)
            if(BuildForCore2_X86_64)
                set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptions_X86_64Core2})
            else(BuildForCore2_X86_64)
                set(CMAKE_CXX_FLAGS_RELEASE ${BuildOptionsBasic})
            endif(BuildForCore2_X86_64)
        endif(BuildForAMD_X86_64)
        message(STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_RELEASE}")
    endif(BuildForDebug)
    add_definitions(-fPIC)
endif(MSVC)

#####################################################################################
### BEGIN : Core ZynAddSubFx code
#   - needed for all executables and libraries in this cmakelists
add_subdirectory(Misc)
add_subdirectory(Synth)
add_subdirectory(Effects)
add_subdirectory(Params)
add_subdirectory(DSP)

add_library(zynaddsubfx_core STATIC
    ${zynaddsubfx_dsp_SRCS}
    ${zynaddsubfx_effect_SRCS}
    ${zynaddsubfx_misc_SRCS}
    ${zynaddsubfx_params_SRCS}
    ${zynaddsubfx_synth_SRCS}
    )

target_link_libraries(zynaddsubfx_core
    ${FFTW_LIBRARIES}
    ${MXML_LIBRARIES}
    ${PTHREAD_LIBRARIES}
    ${OS_LIBRARIES}
    )

add_subdirectory(Nio)
### END : Core ZynAddSubFx code
#####################################################################################


#####################################################################################
### BEGIN : ZynMusicStudio code (ZMS)
#   - needed for zynmusicstudio executable
message(STATUS "Building ZynMusicStudio executable")
add_subdirectory(ZMS)

include_directories(${ZMS_INCLUDE_DIRS})

add_executable(zynmusicstudio zynmusicstudio.cpp)

target_link_libraries(zynmusicstudio
	zynaddsubfx_core
	zynaddsubfx_nio
	zynmusicstudio_zms
	${NIO_LIBRARIES}
	)
### END : ZynMusicStudio code (ZMS)
#####################################################################################
