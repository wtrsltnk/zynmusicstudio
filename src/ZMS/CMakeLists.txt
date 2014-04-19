set(zynmusicstudio_zms_SRCS
    zynapplication.cpp
    mainwindow.cpp
    timesignature.cpp
    viewscale.cpp
    
    pianoroll/pianorollwidget.cpp
    pianoroll/pianorolltool.cpp
    pianoroll/pianorollselection.cpp
    pianoroll/pianorollnoteitem.cpp
    
    sequencer/sequencer.cpp
    sequencer/sequencersong.cpp
    sequencer/sequencerclip.cpp
    sequencer/sequencernote.cpp
    sequencer/sequencertrack.cpp
    sequencer/sequencerautomatedproperty.cpp
    sequencer/sequencerautomatedpropertypoint.cpp
    
    trackarea/trackareawidget.cpp
    trackarea/trackareaclip.cpp
    trackarea/trackproperties.cpp
    trackarea/trackareaselection.cpp
    trackarea/trackareatool.cpp
    trackarea/trackareaselectchanneldialog.cpp

    channel/channeleditorwidget.cpp
    channel/channelstripwidget.cpp
    channel/channelstripcontainer.cpp
    channel/masterchannelstripwidget.cpp
    channel/addsyntheditorwidget.cpp
    channel/subsyntheditorwidget.cpp
    channel/padsyntheditorwidget.cpp
    channel/effectbuttonstrip.cpp

    mixer/mixermaster.cpp
    mixer/mixerchannel.cpp
    )
set(zynmusicstudio_zms_HEADERS
    mainwindow.h
    timesignature.h
    viewscale.h
    
    pianoroll/pianorollwidget.h
    pianoroll/pianorolltool.h
    pianoroll/pianorollselection.h
    pianoroll/pianorollnoteitem.h
    
    sequencer/sequencer.h
    sequencer/sequencersong.h
    sequencer/sequencerclip.h
    sequencer/sequencernote.h
    sequencer/sequencertrack.h
    sequencer/sequencerautomatedproperty.h
    sequencer/sequencerautomatedpropertypoint.h
    
    trackarea/trackareawidget.h
    trackarea/trackareaclip.h
    trackarea/trackproperties.h
    trackarea/trackareaselection.h
    trackarea/trackareatool.h
    trackarea/trackareaselectchanneldialog.h

    channel/channeleditorwidget.h
    channel/channelstripwidget.h
    channel/channelstripcontainer.h
    channel/masterchannelstripwidget.h
    channel/addsyntheditorwidget.h
    channel/subsyntheditorwidget.h
    channel/padsyntheditorwidget.h
    channel/effectbuttonstrip.h

    mixer/mixermaster.h
    mixer/mixerchannel.h
    )
set(zynmusicstudio_zms_FORMS
    mainwindow.ui

    pianoroll/pianorollwidget.ui

    trackarea/trackareawidget.ui
    trackarea/trackproperties.ui
    trackarea/trackareaselectchanneldialog.ui

    channel/channeleditorwidget.ui
    channel/channelstripwidget.ui
    channel/channelstripcontainer.ui
    channel/masterchannelstripwidget.ui
    channel/addsyntheditorwidget.ui
    channel/subsyntheditorwidget.ui
    channel/padsyntheditorwidget.ui
    channel/effectbuttonstrip.ui
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# QT library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
qt5_wrap_cpp(zynmusicstudio_zms_HEADERS_MOC ${zynmusicstudio_zms_HEADERS})
qt5_wrap_ui(zynmusicstudio_zms_FORMS_HEADERS ${zynmusicstudio_zms_FORMS})

message(STATUS "bin : ${CMAKE_CURRENT_BINARY_DIR}")

# Include cmake binary directory for all MOC headers
set(LOCAL_INCLUDE_DIRS
    
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    )

set(ZMS_INCLUDE_DIRS ${LOCAL_INCLUDE_DIRS} PARENT_SCOPE)

qt5_wrap_cpp(zynmusicstudio_zms_HEADERS_MOC ${zynmusicstudio_zms_HEADERS})
qt5_wrap_ui(zynmusicstudio_zms_FORMS_HEADERS ${zynmusicstudio_zms_FORMS})

add_library(zynmusicstudio_zms STATIC
    ${zynmusicstudio_zms_SRCS}
    ${zynmusicstudio_zms_HEADERS_MOC}
    ${zynmusicstudio_zms_FORMS_HEADERS}
    )

qt5_use_modules(zynmusicstudio_zms Widgets)
