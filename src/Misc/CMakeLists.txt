set(zynaddsubfx_misc_SRCS
    Misc/Bank.cpp
    Misc/Config.cpp
    Misc/Dump.cpp
    Misc/Instrument.cpp
    Misc/Drumkit.cpp
    Misc/Master.cpp
    Misc/Microtonal.cpp
    Misc/Util.cpp
    Misc/XMLwrapper.cpp
    Misc/WavFile.cpp
    Misc/WaveShapeSmps.cpp
    )

if(LashEnable)
    set(zynaddsubfx_misc_SRCS
        ${zynaddsubfx_misc_SRCS}
        Misc/LASHClient.cpp
        PARENT_SCOPE)
else()
    set(zynaddsubfx_misc_SRCS
        ${zynaddsubfx_misc_SRCS}
        PARENT_SCOPE)
endif()
