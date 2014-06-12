set(zynmusicstudio_ui_SRCS
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

    channel/channeleditorwidget.cpp
    channel/channelstripwidget.cpp
    channel/channelstripcontainer.cpp
    channel/masterchannelstripwidget.cpp
    channel/addsyntheditorwidget.cpp
    channel/subsyntheditorwidget.cpp
    channel/padsyntheditorwidget.cpp
    channel/effectbuttonstrip.cpp
    channel/inserteffectbuttonstrip.cpp
    channel/sendbuttonstrip.cpp
    
    effects/inserteffectswidget.cpp
    effects/effectwidget.cpp
    effects/alien_effectwidget.cpp
    effects/chorus_effectwidget.cpp
    effects/distortion_effectwidget.cpp
    effects/dynamic_effectwidget.cpp
    effects/echo_effectwidget.cpp
    effects/eq_effectwidget.cpp
    effects/phaser_effectwidget.cpp
    effects/reverb_effectwidget.cpp

    mixer/mixer.cpp
    mixer/mixerbuffer.cpp
    mixer/mixerchannelinput.cpp
    mixer/mixerinstrument.cpp
    mixer/mixermaster.cpp
    mixer/mixereffect.cpp
    mixer/mixereffectcontainer.cpp
    mixer/mixerinserteffect.cpp
    mixer/mixerinserteffectcontainer.cpp
    mixer/mixerchannel.cpp
    mixer/mixerbus.cpp
    mixer/mixersendsource.cpp
    mixer/mixersendsink.cpp

    library/library.cpp
    library/libraryplugin.cpp
    library/libraryeqeffectplugin.cpp
    library/libraryeqeffect.cpp
    )
set(zynmusicstudio_ui_HEADERS
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

    channel/channeleditorwidget.h
    channel/channelstripwidget.h
    channel/channelstripcontainer.h
    channel/masterchannelstripwidget.h
    channel/addsyntheditorwidget.h
    channel/subsyntheditorwidget.h
    channel/padsyntheditorwidget.h
    channel/effectbuttonstrip.h
    channel/inserteffectbuttonstrip.h
    channel/sendbuttonstrip.h
    
    effects/inserteffectswidget.h
    effects/effectwidget.h
    effects/alien_effectwidget.h
    effects/chorus_effectwidget.h
    effects/distortion_effectwidget.h
    effects/dynamic_effectwidget.h
    effects/echo_effectwidget.h
    effects/eq_effectwidget.h
    effects/phaser_effectwidget.h
    effects/reverb_effectwidget.h

    mixer/mixer.h
    mixer/mixerchannelinput.h
    mixer/mixerinstrument.h
    mixer/mixerbuffer.h
    mixer/mixereffect.h
    mixer/mixereffectcontainer.h
    mixer/mixerinserteffect.h
    mixer/mixerinserteffectcontainer.h
    mixer/mixermaster.h
    mixer/mixerchannel.h
    mixer/mixerbus.h
    mixer/mixersendsource.h
    mixer/mixersendsink.h

    library/library.h
    library/libraryplugin.h
    library/libraryeqeffectplugin.h
    library/libraryeqeffect.h
    )
set(zynmusicstudio_ui_FORMS
    mainwindow.ui

    pianoroll/pianorollwidget.ui

    trackarea/trackareawidget.ui
    trackarea/trackproperties.ui

    channel/channeleditorwidget.ui
    channel/channelstripwidget.ui
    channel/channelstripcontainer.ui
    channel/masterchannelstripwidget.ui
    channel/addsyntheditorwidget.ui
    channel/subsyntheditorwidget.ui
    channel/padsyntheditorwidget.ui
    channel/effectbuttonstrip.ui
    channel/inserteffectbuttonstrip.ui
    channel/sendbuttonstrip.ui
    
    effects/inserteffectswidget.ui
    effects/effectwidget.ui
    effects/alien_effectwidget.ui
    effects/chorus_effectwidget.ui
    effects/distortion_effectwidget.ui
    effects/dynamic_effectwidget.ui
    effects/echo_effectwidget.ui
    effects/eq_effectwidget.ui
    effects/phaser_effectwidget.ui
    effects/reverb_effectwidget.ui
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# QT library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
qt5_wrap_cpp(zynmusicstudio_ui_HEADERS_MOC ${zynmusicstudio_ui_HEADERS})
qt5_wrap_ui(zynmusicstudio_ui_FORMS_HEADERS ${zynmusicstudio_ui_FORMS})

message(STATUS "bin : ${CMAKE_CURRENT_BINARY_DIR}")

# Include cmake binary directory for all MOC headers
set(LOCAL_INCLUDE_DIRS
    
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    )

set(UI_INCLUDE_DIRS ${LOCAL_INCLUDE_DIRS} PARENT_SCOPE)

qt5_wrap_cpp(zynmusicstudio_ui_HEADERS_MOC ${zynmusicstudio_ui_HEADERS})
qt5_wrap_ui(zynmusicstudio_ui_FORMS_HEADERS ${zynmusicstudio_ui_FORMS})

add_library(zynmusicstudio_ui STATIC
    ${zynmusicstudio_ui_SRCS}
    ${zynmusicstudio_ui_HEADERS_MOC}
    ${zynmusicstudio_ui_FORMS_HEADERS}
    )

qt5_use_modules(zynmusicstudio_ui Widgets)
