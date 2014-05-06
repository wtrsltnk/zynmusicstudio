/*
  ZynAddSubFX - a software synthesizer

  mainwindow.cpp - Mainwindow voor Zyn Music Studio
  Copyright (c) 2002-2009 Nasca Octavian Paul
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink, Nasca Octavian Paul

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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QToolBar>
#include <iostream>
#include "sequencer/sequencer.h"
#include "channel/channelstripwidget.h"
#include "mixer/mixer.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->trackareawidget->_mainParent = this;
    this->ui->channelsContainer->_mainParent = this;

    connect(&this->ui->trackareawidget->Selection(), SIGNAL(SelectionChanged(QList<TrackAreaClip*>)),
            this->ui->pianoroll, SLOT(ClipSelectionChanged(QList<TrackAreaClip*>)));
    connect(this->ui->trackareawidget, SIGNAL(ActivateClip(TrackAreaClip*)), this, SLOT(ClipIsActivated(TrackAreaClip*)));

    connect(this->ui->btnPlay, SIGNAL(clicked()), this, SLOT(OnPlay()));
    connect(this->ui->btnPause, SIGNAL(clicked()), this, SLOT(OnPause()));
    connect(this->ui->btnStop, SIGNAL(clicked()), this, SLOT(OnStop()));

    Sequencer::Inst().NewSong();
    Sequencer::Inst().CurrentSong()->AddTrack();
    Sequencer::Inst().CurrentSong()->AddTrack();

    Sequencer::Inst().CurrentSongIsUpdated();

    connect(this->ui->actionChannels_pannel, SIGNAL(toggled(bool)), this->ui->channelsDock, SLOT(setVisible(bool)));
    connect(this->ui->actionPianoroll_panel, SIGNAL(toggled(bool)), this->ui->pianorollDock, SLOT(setVisible(bool)));

    this->ui->actionChannels_pannel->setChecked(this->ui->channelsDock->isVisible());
    this->ui->actionPianoroll_panel->setChecked(this->ui->pianorollDock->isVisible());

    MixerChannel* channel = Mixer::Instance().AddChannel("wouter");
    channel->SetSink(Mixer::Instance().Master());
    MixerInstrument* instrument = Mixer::Instance().AddInstrument("test");
    channel->SetChannelInput(instrument);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClipIsActivated(TrackAreaClip* clip)
{
    this->ui->pianorollDock->show();
    QList<TrackAreaClip*> selection;
    selection.push_back(clip);
    this->ui->pianoroll->ClipSelectionChanged(selection);
}

void MainWindow::OnPlay()
{
    Sequencer::Inst().Start();
}

void MainWindow::OnPause()
{
    Sequencer::Inst().Pause();
}

void MainWindow::OnStop()
{
    Sequencer::Inst().Stop();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
}
