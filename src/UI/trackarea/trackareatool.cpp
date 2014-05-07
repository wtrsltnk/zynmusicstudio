#include "trackareatool.h"
#include "sequencer/sequencer.h"
#include <iostream>

using namespace std;

TrackAreaTool::TrackAreaTool(TrackAreaWidget* trackarea)
    : _trackarea(trackarea), _mode(TrackAreaToolMode::None), _movegroup(0)
{ }

TrackAreaTool::~TrackAreaTool()
{ }

void TrackAreaTool::OnMousePress(QGraphicsSceneMouseEvent* e)
{
    this->_startPos = e->scenePos();
    QList<QGraphicsItem*> items = this->_trackarea->Scene()->items(this->_startPos, Qt::IntersectsItemShape, Qt::AscendingOrder);

    if (e->buttons().testFlag(Qt::LeftButton))
    {
        TrackAreaSelection& sel = this->_trackarea->Selection();
        bool done = false;
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            if ((*i)->data(23).toString() == "trackareaclip" && (*i)->group() != 0)
            {
                if (e->modifiers().testFlag(Qt::ControlModifier))
                {
                    sel.ToggleSelection(dynamic_cast<TrackAreaClip*> ((*i)->group()));
                    if (sel.IsSelected(dynamic_cast<TrackAreaClip*> ((*i)->group())))
                    {
                        this->_mode = TrackAreaToolMode::Move;
                    }
                    else
                    {
                        this->_mode = TrackAreaToolMode::None;
                    }
                }
                else if (e->modifiers().testFlag(Qt::ShiftModifier))
                {
                    this->_mode = TrackAreaToolMode::Copy;
                }
                else if (sel.IsSelected(dynamic_cast<TrackAreaClip*> ((*i)->group())) == false)
                {
                    sel.StartSelection(dynamic_cast<TrackAreaClip*> ((*i)->group()));
                    this->_mode = TrackAreaToolMode::Move;
                }
                else
                {
                    this->_mode = TrackAreaToolMode::Move;
                }

                done = true;
            }
            else if ((*i)->data(23).toString() == "handle" && (*i)->group() != 0)
            {
                this->_mode = TrackAreaToolMode::Create;
                this->_createClip = dynamic_cast<TrackAreaClip*> ((*i)->group());
                done = true;
            }
        }

        if (done == false)
        {
            this->SetupSelectionRect(TrackAreaToolMode::Select);
        }
    }
    else if (e->buttons().testFlag(Qt::RightButton))
    {
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            if ((*i)->data(23).toString() == "trackareaclip" && (*i)->group() != 0)
            {
                TrackAreaClip* clip = dynamic_cast<TrackAreaClip*> ((*i)->group());
                if (clip != 0)
                {
                    this->_trackarea->RemoveClip(clip);
                    delete clip;
                }
            }
        }
    }

    this->_trackarea->UpdateTracks();
}

#define fmin(a,b) a < b ? a : b
#define fmax(a,b) a > b ? a : b
void TrackAreaTool::OnMouseMove(QGraphicsSceneMouseEvent* e)
{
    if (this->_mode == TrackAreaToolMode::Select && this->_selectionRect != 0)
    {
        QPointF amin(fmin(e->scenePos().x(), this->_startPos.x()), fmin(e->scenePos().y(), this->_startPos.y()));
        QPointF amax(fmax(e->scenePos().x(), this->_startPos.x()), fmax(e->scenePos().y(), this->_startPos.y()));
        this->_selectionRect->setRect(amin.x(), amin.y(), amax.x()-amin.x(), amax.y()-amin.y());
        this->_trackarea->UpdateTracks();
    }
    else if (this->_mode == TrackAreaToolMode::Create && this->_createClip != 0)
    {
        if (e->scenePos().x() >= (this->_createClip->GetClip()->Start() + this->_createClip->GetClip()->Length()) * this->_trackarea->LocalViewScale().horizontal())
        {
            SequencerClip* newclip = this->_createClip->GetClip()->GetTrack()->CloneClip(this->_createClip->GetClip());
            newclip->SetStart(this->_createClip->GetClip()->Start() + this->_createClip->GetClip()->Length());
            this->_trackarea->UpdateTracks();
            this->_createClip = this->_trackarea->ClipBySequencerClip(newclip);
        }
    }
    else if (this->_mode == TrackAreaToolMode::Move || this->_mode == TrackAreaToolMode::Copy)
    {
        if (this->_movegroup == 0)
            this->SetupEditGroupFromSelection();
        int startindex = int(this->_startPos.y() / this->_trackarea->LocalViewScale().vertical());
        int newindex = int(e->scenePos().y() / this->_trackarea->LocalViewScale().vertical());

        if (newindex < 0)
            newindex = 0;
        if (newindex >= Sequencer::Inst().CurrentSong()->Tracks().size())
            newindex = Sequencer::Inst().CurrentSong()->Tracks().size() - 1;

        double diffx = e->scenePos().x() - this->_startPos.x();
        diffx = diffx - (int(diffx) % this->_trackarea->LocalViewScale().horizontal());

        while (this->_movegroup->boundingRect().x() + diffx <= -this->_trackarea->LocalViewScale().horizontal())
            diffx += this->_trackarea->LocalViewScale().horizontal();

        this->_movegroup->setTransform(QTransform::fromTranslate(diffx, (newindex - startindex) * this->_trackarea->LocalViewScale().vertical()));
    }
}

void TrackAreaTool::OnMouseRelease(QGraphicsSceneMouseEvent* e)
{
    TrackAreaSelection& sel = this->_trackarea->Selection();
    if (this->_mode == TrackAreaToolMode::Select && this->_selectionRect != 0)
    {
        QList<QGraphicsItem*> items = this->_trackarea->Scene()->items(this->_selectionRect->rect());
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            if ((*i)->data(23).toString() == "trackareaclip" && (*i)->group() != 0)
            {
                if (e->modifiers().testFlag(Qt::ControlModifier))
                    sel.ToggleSelection(dynamic_cast<TrackAreaClip*> ((*i)->group()));
                else
                    sel.AddToSelection(dynamic_cast<TrackAreaClip*> ((*i)->group()));
            }
        }
        this->CleanupSelectionRect();
    }
    else if (this->_mode == TrackAreaToolMode::Move && this->_movegroup != 0)
    {
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            TrackAreaClip* item = (TrackAreaClip*)((QGraphicsItem*)*i)->data(22).value<void*>();
            item->UpdateFromRect(this->_movegroup->transform().mapRect(((QGraphicsRectItem*)*i)->rect()));
        }
        this->CleanupEditGroup();
    }
    else if (this->_mode == TrackAreaToolMode::Copy && this->_movegroup != 0)
    {
        sel.Reset();
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            TrackAreaClip* item = (TrackAreaClip*)((QGraphicsItem*)*i)->data(22).value<void*>();
            TrackAreaClip* newitem = item->Clone();
            this->_trackarea->Scene()->addItem(newitem);
            newitem->UpdateFromRect(this->_movegroup->transform().mapRect(((QGraphicsRectItem*)*i)->rect()));
            sel.AddToSelection(newitem);
        }
        this->CleanupEditGroup();
    }
    else if (this->_mode == TrackAreaToolMode::Create && this->_createClip != 0)
    {
        this->_createClip = 0;
    }
    this->_mode = TrackAreaToolMode::None;
    this->_trackarea->UpdateTracks();
}

void TrackAreaTool::OnMouseDoubleClick(QGraphicsSceneMouseEvent* e)
{
    QList<QGraphicsItem*> items = this->_trackarea->Scene()->items(this->_startPos);
    for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
    {
        if ((*i)->data(23).toString() == "trackareaclip" && (*i)->group() != 0)
        {
            this->_trackarea->ActivateClip(dynamic_cast<TrackAreaClip*> ((*i)->group()));
            return;
        }
    }

    int trackindex = int(e->scenePos().y() / this->_trackarea->LocalViewScale().vertical());
    SequencerTrack* track = Sequencer::Inst().CurrentSong()->GetTrackAtIndex(trackindex);
    if (track != 0)
    {
        SequencerClip* clip = track->AddClip();
        double diffx = e->scenePos().x() - (int(e->scenePos().x()) % this->_trackarea->LocalViewScale().horizontal());
        clip->SetStart(diffx / this->_trackarea->LocalViewScale().horizontal());
        this->_trackarea->UpdateTracks();
        this->_trackarea->ActivateClip(this->_trackarea->ClipBySequencerClip(clip));
    }
}

void TrackAreaTool::SetupEditGroupFromSelection()
{
    this->_movegroup = new QGraphicsItemGroup();
    this->_movegroup->setZValue(10);
    for (QList<TrackAreaClip*>::const_iterator i = this->_trackarea->Selection().Clips().cbegin();i != this->_trackarea->Selection().Clips().cend(); ++i)
    {
        TrackAreaClip* item = (TrackAreaClip*)*i;
        QGraphicsRectItem* rect = new QGraphicsRectItem();
        rect->setRect(item->RectItem()->rect());
        rect->setData(22, QVariant::fromValue((void*)item));
        rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 55)));
        rect->setPen(QPen(Qt::transparent));
        this->_movegroup->addToGroup(rect);
    }
    this->_trackarea->Scene()->addItem(this->_movegroup);
}

void TrackAreaTool::CleanupEditGroup()
{
    this->_trackarea->Scene()->removeItem(this->_movegroup);
    delete this->_movegroup;
    this->_movegroup = 0;
}

void TrackAreaTool::SetupSelectionRect(TrackAreaToolMode::eType mode)
{
    this->_mode = mode;
    this->_selectionRect = new QGraphicsRectItem();
    this->_selectionRect->setRect(this->_startPos.x(), this->_startPos.y(), 1, 1);
    this->_selectionRect->setPen(QPen(QColor::fromRgb(155, 255, 155, 155)));
    this->_selectionRect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 55)));
    this->_selectionRect->setZValue(50);
    this->_trackarea->Scene()->addItem(this->_selectionRect);
    this->_trackarea->Selection().Reset();
}

void TrackAreaTool::CleanupSelectionRect()
{
    this->_trackarea->Scene()->removeItem(this->_selectionRect);
    delete this->_selectionRect;
    this->_selectionRect = 0;
}
