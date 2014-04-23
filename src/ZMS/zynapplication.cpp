/*
  ZynAddSubFX - a software synthesizer

  zynapplication.cpp - The QApplication for running zyn music studio
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
#include "zynapplication.h"
#include "mainwindow.h"
#include "../ZMS/mixer/mixer.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

ZynApplication::ZynApplication(int argc, char *argv[]) :
    QApplication(argc, argv)
{
    this->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(53, 53, 53));
    p.setColor(QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::Highlight, QColor(0, 143, 191));
    p.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    p.setColor(QPalette::WindowText, QColor(255, 255, 255));
    p.setColor(QPalette::WindowText, QColor(255, 255, 255));
    this->setPalette(p);

    this->setStyleSheet(
                "QSlider {"
                "     background: #353535;"
                "}"

                "#trackproperties {"
                "     background: #555555;"
                "    border: 1px solid #2E2E2E;"
                "    border-right: 0px;"
                "}"

                "#clips {"
                "    border: 1px solid #2E2E2E;"
                "    border-left: 0px;"
                "}"

                "EffectButtonStrip #buttons {"
                "   background-color: #555555;"
                "}"

                "TrackProperties {"
                "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #494949, stop:0.5 #444444, stop:1 #3F3F3F);"
                "    border-top: 1px solid #555555;"
                "    border-bottom: 1px solid #2E2E2E;"
                "}"

                "ChannelStripWidget, MasterChannelStripWidget {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #494949, stop:0.5 #444444, stop:1 #3F3F3F);"
                "    border-right: 1px solid #2E2E2E;"
                "}"

                "#filler {"
                "    background: #353535;"
                "    height: 15px;"
                "    margin: 0px;"
                "    border: 1px solid #2E2E2E;"
                "    border-top: 0px;"
                "    border-right: 0px;"
                "}"

                "#filler2 {"
                "    background: #353535;"
                "    height: 15px;"
                "    margin: 0px;"
                "    border: 1px solid #2E2E2E;"
                "    border-bottom: 0px;"
                "    border-right: 0px;"
                "}"

                "#horizontalSlider {"
                "    background: #353535;"
                "    border-top: 1px solid #2E2E2E;"
                "}"

                "#verticalSlider {"
                "    background: #353535;"
                "    border-left: 1px solid #2E2E2E;"
                "}"

                "QScrollBar:horizontal {"
                "    background: #353535;"
                "    height: 15px;"
                "    margin: 0px;"
                "    border: 1px solid #2E2E2E;"
                "    border-top: 0px;"
                "    border-left: 0px;"
                "}"

                "QScrollBar:vertical {"
                "    background: #353535;"
                "    width: 15px;"
                "    margin: 0px;"
                "    border: 0px;"
                "}"

                "QScrollBar::handle:horizontal {"
                "    background: #777777;"
                "    min-width: 20px;"
                "    border-radius: 4px;"
                "    margin: 2px;"
                "}"

                "QScrollBar::handle:vertical {"
                "    background: #777777;"
                "    min-height: 20px;"
                "    border-radius: 4px;"
                "    margin: 2px;"
                "}"

                "QScrollBar::add-line, QScrollBar::sub-line {"
                "    width: 0px;"
                "}"

                "#line, #line_2, #line_3 {"
                "    color: #777777;"
                "}"

                "QTabBar::tab:disabled {"
                "     color:#858585; /* make non-selected tabs look smaller */"
                "}"
                        );
}

ZynApplication::~ZynApplication()
{
    Mixer::Instance().Lock();
    Mixer::Instance().Unlock();

    Mixer::Instance().EngineManager()->stop();
}

int ZynApplication::exec()
{
    if (Mixer::Instance().EngineManager()->start())
    {
        MainWindow wnd;
        wnd.show();
        return QApplication::exec();
    }
    return 0;
}
