#include "trackareawidget.h"
#include "ui_trackareawidget.h"
#include "sequencer/sequencer.h"
#include "trackareaclip.h"
#include "trackareatool.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QGraphicsRectItem>
#include <iostream>

using namespace std;

TrackAreaWidget::TrackAreaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackAreaWidget), _mainParent(0)
{
    ui->setupUi(this);

    this->_viewScale.setVerticalScale(60);

    this->_currentTool = new TrackAreaTool(this);

    connect(&Sequencer::Inst(), SIGNAL(CurrentSongIsUpdated()), this, SLOT(UpdateTracks()));
    connect(&Sequencer::Inst(), SIGNAL(SelectedTrackChanged(SequencerTrack*)), this, SLOT(UpdateTracks()));

    connect(this->ui->clips->verticalScrollBar(), SIGNAL(valueChanged(int)), this->ui->trackproperties->verticalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->trackproperties->verticalScrollBar(), SIGNAL(valueChanged(int)), this->ui->clips->verticalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this->ui->clips->horizontalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(UpdateTracks()));
    connect(this->ui->clips->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this->ui->horizontalScrollBar, SLOT(setRange(int,int)));

    connect(this->ui->horizontalSlider, SIGNAL(valueChanged(int)), &this->_viewScale, SLOT(setHorizontalScale(int)));
    connect(this->ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateTracks()));
    connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(invertSlider(int)));

    this->_scene = new QGraphicsScene();
    this->_scene->installEventFilter(this);
    this->ui->clips->setScene(this->_scene);

    this->_clips = new QGraphicsItemGroup();
    this->_clips->setData(23, "TrackAreaWidget-clips");
    this->_clips->setFiltersChildEvents(false);
    this->_clips->setHandlesChildEvents(false);
    this->_scene->addItem(this->_clips);
    this->_clips->setZValue(3);

    this->_tracks = new QGraphicsItemGroup();
    this->_tracks->setData(23, "TrackAreaWidget-tracks");
    this->_tracks->setFiltersChildEvents(false);
    this->_tracks->setHandlesChildEvents(false);
    this->_scene->addItem(this->_tracks);
    this->_tracks->setZValue(2);

    this->ui->timeline->installEventFilter(this);
}

TrackAreaWidget::~TrackAreaWidget()
{
    delete ui;
}

void TrackAreaWidget::invertSlider(int value)
{
    this->_viewScale.setVerticalScale(this->ui->verticalSlider->minimum() + this->ui->verticalSlider->maximum() - value);
    this->UpdateTracks();
}

bool TrackAreaWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this->_scene)
    {
        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
                this->_currentTool->OnMousePress(e);
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
            {
                this->_currentTool->OnMouseRelease(e);
                Sequencer::Inst().CurrentSongIsUpdated();
            }
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
                this->_currentTool->OnMouseMove(e);
        }
        else if (event->type() == QEvent::GraphicsSceneMouseDoubleClick)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
                this->_currentTool->OnMouseDoubleClick(e);
        }
    }
    else if (obj == this->ui->timeline)
    {
        if (event->type() == QEvent::Paint)
        {
            QPainter p(this->ui->timeline);
            p.fillRect(0, 0, this->ui->timeline->width(), this->ui->timeline->height(), QColor(85, 85, 85));

            // Draw timeline
            p.setFont(QFont("Verdana", 10));
            for (int x = 0; (x * this->_viewScale.horizontal()) < this->Scene()->width() + this->ui->timeline->width(); x++)
            {
                if (x == 0)
                    p.setPen(QPen(QColor(46, 46, 46)));
                else if (x % Sequencer::Inst().LocalTimeSignature().numBeats() == 0)
                    p.setPen(QPen(QColor(56, 56, 56)));
                else
                    p.setPen(QPen(QColor(63, 63, 63)));

                if (x % Sequencer::Inst().LocalTimeSignature().numBeats())
                {
                    p.drawLine((x * this->_viewScale.horizontal()) - this->ui->horizontalScrollBar->value(),
                               this->ui->timeline->height()/2,
                               (x * this->_viewScale.horizontal()) - this->ui->horizontalScrollBar->value(),
                               this->ui->timeline->height());
                }
                else
                {
                    p.drawLine((x * this->_viewScale.horizontal()) - this->ui->horizontalScrollBar->value(),
                               0,
                               (x * this->_viewScale.horizontal()) - this->ui->horizontalScrollBar->value(),
                               this->ui->timeline->height());
                    p.setPen(QPen(QColor(246, 246, 246)));
                    p.drawText((x * this->_viewScale.horizontal()) - this->ui->horizontalScrollBar->value() + 2,
                               p.fontInfo().pointSize() + 2,
                               QString::number(x+1));
                }
            }
            return true;
        }
    }
    return false;
}

void TrackAreaWidget::UpdateTracks()
{
    int w = this->_viewScale.horizontal() * 16;
    int h = this->LocalViewScale().vertical();

    QPixmap bg(w, h * 2);
    QPainter painter(&bg);
    painter.fillRect(0, 0, w, h, QColor(85, 85, 85));
    painter.fillRect(0, h, w, h, QColor(80, 80, 80));

    painter.setPen(QPen(QColor(46, 46, 46)));
    painter.drawLine(0, h-1, w, h-1);
    painter.drawLine(0, (h*2)-1, w, (h*2)-1);

    for (int i = 0; i < Sequencer::Inst().LocalTimeSignature().numBeats() * 4; i++)
    {
        if (i == 0)
            painter.setPen(QPen(QColor(46, 46, 46)));
        else if (i % Sequencer::Inst().LocalTimeSignature().numBeats() == 0)
            painter.setPen(QPen(QColor(56, 56, 56)));
        else
            painter.setPen(QPen(QColor(63, 63, 63)));

        painter.drawLine(this->_viewScale.horizontal() * i, 0, this->_viewScale.horizontal() * i, (h*2));
    }
    this->ui->clips->setBackgroundBrush(QBrush(bg));

    this->ui->timeline->repaint();

    if (Sequencer::Inst().CurrentSong() == 0)
        return;

    if (Sequencer::Inst().CurrentSong()->Tracks().size() == 0)
        return;

    this->ui->trackproperties->widget()->setFixedHeight(Sequencer::Inst().CurrentSong()->Tracks().size() * this->LocalViewScale().vertical());

    double width = 1.0;
    int j = 0;

    // First clean-up all tracks which are invalid
    QVBoxLayout* tp = ((QVBoxLayout*)this->ui->scrollAreaWidgetContents->layout());
    for (int i = 0; i < tp->count(); i++)
    {
        QLayoutItem* item = tp->itemAt(i);
        TrackProperties* trackproperties = (TrackProperties*)item->widget();
        SequencerTrack* track = trackproperties->GetTrack();
        // When the track is not found in the current song anymore
        if (Sequencer::Inst().CurrentSong()->Tracks().indexOf((SequencerTrack * const) track) == -1)
        {
            for (QList<SequencerClip*>::iterator k = track->Clips().begin();
                 k != track->Clips().end(); ++k)
            {
                TrackAreaClip* clip = this->ClipBySequencerClip(*k);
                if (clip != 0)
                {
                    this->_clips->removeFromGroup(clip);
                    delete clip;
                }
            }
            // We remove the widget and delete it.
            tp->removeWidget(item->widget());
            delete trackproperties;
        }
    }

    // Now update all tracks that are left
    for (QList<SequencerTrack*>::iterator i = Sequencer::Inst().CurrentSong()->Tracks().begin();
         i != Sequencer::Inst().CurrentSong()->Tracks().end(); ++i)
    {
        SequencerTrack* track = *i;
        TrackProperties* p = this->TrackPropertiesBySequencerTrack(track);
        if (p == 0)
        {
            p = new TrackProperties(track);
            tp->addWidget(p);
            if (this->_mainParent != 0)
                connect(p, SIGNAL(ActivateChannel(Instrument*)), this->_mainParent, SLOT(OnChannelIsActivated(Instrument*)));
        }
        p->UpdateWidget();

        for (QList<SequencerClip*>::iterator k = track->Clips().begin();
             k != track->Clips().end(); ++k)
        {
            if ((*k)->Start() + (*k)->Length() > width)
                width = (*k)->Start() + (*k)->Length() + 4.0;

            TrackAreaClip* clip = this->ClipBySequencerClip(*k);
            if (clip == 0)
            {
                clip = new TrackAreaClip(this, *k);
                this->_clips->addToGroup(clip);
            }
            clip->Update(j);
        }
        j++;
    }

    this->_scene->setSceneRect(0, 0,
                               width * this->LocalViewScale().horizontal(),
                               Sequencer::Inst().CurrentSong()->Tracks().size() * this->LocalViewScale().vertical());
}

void TrackAreaWidget::RemoveClip(TrackAreaClip* clip)
{
    this->Selection().Reset();
    clip->GetClip()->GetTrack()->RemoveClip(clip->GetClip());
    this->_clips->removeFromGroup(clip);
}

TrackProperties* TrackAreaWidget::TrackPropertiesBySequencerTrack(SequencerTrack* track)
{
    QVBoxLayout* tp = ((QVBoxLayout*)this->ui->scrollAreaWidgetContents->layout());
    for (int i = 0; i < tp->count(); i++)
    {
        QLayoutItem* item = tp->itemAt(i);
        TrackProperties* trackproperties = (TrackProperties*)item->widget();
        if (trackproperties != 0 && trackproperties->GetTrack() == track)
            return trackproperties;
    }
    return 0;
}

TrackAreaClip* TrackAreaWidget::ClipBySequencerClip(SequencerClip* clip)
{
    QList<QGraphicsItem*> items = this->_scene->items();
    for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
    {
        TrackAreaClip* item = dynamic_cast<TrackAreaClip*>(*i);
        if (item != 0 && item->GetClip() == clip)
            return item;
    }
    return 0;
}
