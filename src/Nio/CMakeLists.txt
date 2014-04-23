add_definitions(-DOUT_DEFAULT="NULL")
add_definitions(-DIN_DEFAULT="NULL")

set(zynaddsubfx_nio_SRCS
    NioInputManager.cpp
    NioOutputManager.cpp
    NioEngineManager.cpp
    NioEngine.cpp
    WavEngine.cpp
    NulEngine.cpp
    RtEngine.cpp
    RtMidi.cpp
    )

find_package(Alsa)
find_package(JACK)
find_package(PORTAUDIO)

set(AlsaEnable ${ALSA_FOUND} CACHE BOOL
    "Enable support for Advanced Linux Sound Architecture")
set(JackEnable ${JACK_FOUND} CACHE BOOL
    "Enable support for JACK Audio Connection toolKit")
set(PaEnable ${PORTAUDIO_FOUND} CACHE BOOL
    "Enable support for Port Audio System")
set(LashEnable ${LASH_FOUND} CACHE BOOL
    "Enable LASH Audio Session Handler")
set(DssiEnable ${DSSI_FOUND} CACHE BOOL
    "Enable DSSI Plugin compilation")
set(LibloEnable ${LIBLO_FOUND} CACHE BOOL
    "Enable Liblo")

if(JackEnable)
message(STATUS "Jack gevonden")
    set(DefaultOutput jack CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    if(AlsaEnable)
        set(DefaultInput alsa CACHE STRING
            "Default Input module: [null, alsa, jack, pa]")
    else()
        set(DefaultInput jack CACHE STRING
            "Default Input module: [null, alsa, jack, pa]")
    endif()
elseif(AlsaEnable)
    set(DefaultOutput alsa CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    set(DefaultInput alsa CACHE STRING
        "Default Input module: [null, alsa, jack, pa]")
elseif(PaEnable)
    set(DefaultOutput pa CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
else()
    set(DefaultOutput null CACHE STRING
        "Default Output module: [null, alsa, jack, pa]")
    set(DefaultInput null CACHE STRING
        "Default Input module: [null, alsa, jack, pa]")
endif()

if(AlsaEnable)
    list(APPEND AUDIO_LIBRARIES ${ASOUND_LIBRARY})
    list(APPEND AUDIO_LIBRARY_DIRS ${ASOUND_LIBRARY_DIRS})
    list(APPEND zynaddsubfx_nio_SRCS AlsaEngine.cpp)
    add_definitions(-DALSA=1)
    message(STATUS "Compiling with alsa")
endif(AlsaEnable)

if(JackEnable)
    include_directories(${JACK_INCLUDE_DIR})
    list(APPEND AUDIO_LIBRARIES ${JACK_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${JACK_LIBRARY_DIRS})
    list(APPEND zynaddsubfx_nio_SRCS JackEngine.cpp)
    add_definitions(-DJACK=1)
    message(STATUS "Compiling with jack")
endif(JackEnable)

if(PaEnable)
    include_directories(${PORTAUDIO_INCLUDE_DIR})
    list(APPEND AUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${PORTAUDIO_LIBRARY_DIRS})
    list(APPEND zynaddsubfx_nio_SRCS PaEngine.cpp)
    add_definitions(-DPORTAUDIO=1)
    message(STATUS "Compiling with portaudio")
endif(PaEnable)

if(LashEnable)
    include_directories(${LASH_INCLUDE_DIRS})
    list(APPEND AUDIO_LIBRARIES ${LASH_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${LASH_LIBRARY_DIRS})
    add_definitions(-DLASH=1)
    message(STATUS "Compiling with lash")
endif(LashEnable)

if(LibloEnable)
    include_directories(${LIBLO_INCLUDE_DIRS})
    list(APPEND AUDIO_LIBRARIES ${LIBLO_LIBRARIES})
    list(APPEND AUDIO_LIBRARY_DIRS ${LIBLO_LIBRARY_DIRS})
    add_definitions(-DUSE_NSM=1)
    message(STATUS "Compiling with liblo")
endif(LibloEnable)

add_library(zynaddsubfx_nio STATIC
    ${zynaddsubfx_nio_SRCS} 
    )

target_link_libraries(zynaddsubfx_nio
    ${AUDIO_LIBRARIES}
    )
