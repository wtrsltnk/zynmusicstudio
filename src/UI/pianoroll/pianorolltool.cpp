#include "pianorolltool.h"
#include <QMouseEvent>
#include <math.h>
#include <iostream>

using namespace std;

PianoRollTool::PianoRollTool(PianoRollWidget* pianoroll)
    : _pianoroll(pianoroll), _mode(PianoRollToolMode::None), _selectionRect(0), _movegroup(0)
{ }

PianoRollTool::~PianoRollTool()
{ }

void PianoRollTool::OnMousePress(QGraphicsSceneMouseEvent* e)
{
    this->_startPos = e->scenePos();
    QList<QGraphicsItem*> items = this->_pianoroll->Scene()->items(this->_startPos);

    if (items.length() > 0)
    {
        if (e->buttons().testFlag(Qt::LeftButton))
        {
            PianoRollSelection& sel = this->_pianoroll->Selection();
            bool done = false;
            for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
            {
                if ((*i)->data(23).toString() == "note" && (*i)->group() != 0)
                {
                    if (e->modifiers().testFlag(Qt::ControlModifier))
                    {
                        sel.ToggleSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
                        if (sel.IsSelected(dynamic_cast<PianoRollNoteItem*> ((*i)->group())))
                            this->SetupEditGroupFromSelection(PianoRollToolMode::Move);
                        else
                            this->_mode = PianoRollToolMode::None;
                    }
                    else if (e->modifiers().testFlag(Qt::ShiftModifier))
                    {
                        this->SetupEditGroupFromSelection(PianoRollToolMode::Copy);
                    }
                    else if (sel.IsSelected(dynamic_cast<PianoRollNoteItem*> ((*i)->group())) == false)
                    {
                        sel.StartSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
                        this->SetupEditGroupFromSelection(PianoRollToolMode::Move);
                    }
                    else
                        this->SetupEditGroupFromSelection(PianoRollToolMode::Move);

                    done = true;
                }
                else if ((*i)->data(23).toString() == "handle" && (*i)->group() != 0)
                {
                    if (sel.IsSelected(dynamic_cast<PianoRollNoteItem*> ((*i)->group())) == false && e->modifiers().testFlag(Qt::ControlModifier))
                        sel.AddToSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
                    else if (sel.IsSelected(dynamic_cast<PianoRollNoteItem*> ((*i)->group())) == false)
                        sel.StartSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
                    this->SetupEditGroupFromSelection(PianoRollToolMode::Size);

                    done = true;
                }
            }

            if (done == false)
            {
                if (e->modifiers().testFlag(Qt::ControlModifier))
                    this->SetupSelectionRect(PianoRollToolMode::Select);
                else
                    this->SetupCreateGroup(PianoRollToolMode::Create, e->scenePos());
            }
        }
        else if (e->buttons().testFlag(Qt::RightButton))
        {
            for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
            {
                if ((*i)->data(23).toString() == "note" && (*i)->group() != 0)
                {
                    PianoRollNoteItem* noteitem = dynamic_cast<PianoRollNoteItem*> ((*i)->group());
                    if (noteitem != 0)
                    {
                        this->_pianoroll->RemoveNoteItem(noteitem);
                        delete noteitem;
                    }
                }
            }
        }
    }
    else
    {
        if (e->modifiers().testFlag(Qt::ControlModifier))
            this->SetupSelectionRect(PianoRollToolMode::Select);
        else
            this->SetupCreateGroup(PianoRollToolMode::Create, e->scenePos());
    }

    this->_pianoroll->UpdateScene();
}

#define fmin(a,b) a < b ? a : b
#define fmax(a,b) a > b ? a : b
void PianoRollTool::OnMouseMove(QGraphicsSceneMouseEvent* e)
{
    double diffx = e->scenePos().x() - this->_startPos.x();
    diffx = diffx - (int(diffx) % this->_pianoroll->LocalViewScale().horizontal());
    double diffy = e->scenePos().y() - this->_startPos.y();
    diffy = diffy - (int(diffy) % this->_pianoroll->LocalViewScale().vertical());

    if (this->_mode == PianoRollToolMode::Select && this->_selectionRect != 0)
    {
        QPointF amin(fmin(e->scenePos().x(), this->_startPos.x()), fmin(e->scenePos().y(), this->_startPos.y()));
        QPointF amax(fmax(e->scenePos().x(), this->_startPos.x()), fmax(e->scenePos().y(), this->_startPos.y()));
        this->_selectionRect->setRect(amin.x(), amin.y(), amax.x()-amin.x(), amax.y()-amin.y());
        this->_pianoroll->UpdateScene();
    }
    else if ((this->_mode == PianoRollToolMode::Move || this->_mode == PianoRollToolMode::Copy) && this->_movegroup != 0)
    {
        // Make sure a note cannot start before time = 0
        while (this->_movegroup->boundingRect().x() + diffx <= -this->_pianoroll->LocalViewScale().horizontal())
            diffx += this->_pianoroll->LocalViewScale().horizontal();

        // Make sure a note value cannot be pushed under 0
        while (this->_movegroup->boundingRect().y() + diffy <= -this->_pianoroll->LocalViewScale().vertical())
            diffy += this->_pianoroll->LocalViewScale().vertical();

        // Make sure a note value cannot be pushed beyond 128
        while (this->_movegroup->boundingRect().y() + diffy >= this->_pianoroll->Scene()->height() - this->_pianoroll->LocalViewScale().vertical())
            diffy -= this->_pianoroll->LocalViewScale().vertical();

        this->_movegroup->setTransform(QTransform::fromTranslate(diffx, diffy));
    }
    else if (this->_mode == PianoRollToolMode::Size && this->_movegroup != 0)
    {
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(*i);
            if (rect != 0)
            {
                PianoRollNoteItem* item = (PianoRollNoteItem*)rect->data(22).value<void*>();
                QRectF r = item->RectItem()->rect();
                rect->setRect(r.x(), r.y(), r.width() + diffx, r.height());
            }
        }
    }
    else if (this->_mode == PianoRollToolMode::Create && this->_movegroup != 0)
    {
        double start = this->_startPos.x() - (int(this->_startPos.x()) % this->_pianoroll->LocalViewScale().horizontal());
        double end = e->scenePos().x() - (int(e->scenePos().x()) % this->_pianoroll->LocalViewScale().horizontal());

        double min = fmin(start, end);
        double max= fmax(start, end);

        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(*i);
            if (rect != 0)
            {
                rect->setRect(min, rect->rect().y(), max-min, rect->rect().height());
            }
        }
    }
}

void PianoRollTool::OnMouseRelease(QGraphicsSceneMouseEvent* e)
{
    PianoRollSelection& sel = this->_pianoroll->Selection();
    if (this->_mode == PianoRollToolMode::Select && this->_selectionRect != 0)
    {
        QList<QGraphicsItem*> items = this->_pianoroll->Scene()->items(this->_selectionRect->rect());
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            if ((*i)->data(23).toString() == "note" && (*i)->group() != 0)
            {
                if (e->modifiers().testFlag(Qt::ControlModifier))
                    sel.ToggleSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
                else
                    sel.AddToSelection(dynamic_cast<PianoRollNoteItem*> ((*i)->group()));
            }
        }
        this->CleanupSelectionRect();
    }
    else if (this->_mode == PianoRollToolMode::Move && this->_movegroup != 0)
    {
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            PianoRollNoteItem* item = (PianoRollNoteItem*)((QGraphicsItem*)*i)->data(22).value<void*>();
            item->UpdateFromRect(this->_movegroup->transform().mapRect(((QGraphicsRectItem*)*i)->rect()));
        }
        this->CleanupEditGroup();
    }
    else if (this->_mode == PianoRollToolMode::Copy && this->_movegroup != 0)
    {
        sel.Reset();
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            PianoRollNoteItem* item = (PianoRollNoteItem*)((QGraphicsItem*)*i)->data(22).value<void*>();
            PianoRollNoteItem* newitem = item->Clone();
            this->_pianoroll->Scene()->addItem(newitem);
            newitem->UpdateFromRect(this->_movegroup->transform().mapRect(((QGraphicsRectItem*)*i)->rect()));
            sel.AddToSelection(newitem);
        }
        this->CleanupEditGroup();
    }
    else if (this->_mode == PianoRollToolMode::Size && this->_movegroup != 0)
    {
        double diffx = e->scenePos().x() - this->_startPos.x();
        diffx = diffx - (int(diffx) % this->_pianoroll->LocalViewScale().horizontal());

        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(*i);
            if (rect != 0)
            {
                PianoRollNoteItem* item = (PianoRollNoteItem*)((QGraphicsItem*)*i)->data(22).value<void*>();
                double len = item->GetNote()->Length() + (diffx / this->_pianoroll->LocalViewScale().horizontal());
                if (len < 0) len = 0;
                item->GetNote()->SetLength(len);
            }
        }
        this->CleanupEditGroup();
    }
    else if (this->_mode == PianoRollToolMode::Create && this->_movegroup != 0)
    {
        QList<QGraphicsItem*> items = this->_movegroup->childItems();
        for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(*i);
            if (rect != 0)
            {
                SequencerNote* note = new SequencerNote(this->_pianoroll->CurrentClip()[0]);
                this->_pianoroll->CurrentClip()[0]->Notes().push_back(note);
                PianoRollNoteItem* item = new PianoRollNoteItem(this->_pianoroll, note);
                item->UpdateFromRect(rect->rect());
                this->_pianoroll->AddNoteToCurrentClip(item);
            }
        }
        this->CleanupCreateGroup();
    }
    this->_pianoroll->UpdateScene();
}

void PianoRollTool::SetupEditGroupFromSelection(PianoRollToolMode::eType mode)
{
    this->_mode = mode;
    if (this->_movegroup != 0)
        this->CleanupEditGroup();
    this->_movegroup = new QGraphicsItemGroup();
    this->_movegroup->setZValue(10);
    for (QList<PianoRollNoteItem*>::const_iterator i = this->_pianoroll->Selection().Notes().cbegin();i != this->_pianoroll->Selection().Notes().cend(); ++i)
    {
        PianoRollNoteItem* item = (PianoRollNoteItem*)*i;
        QGraphicsRectItem* rect = new QGraphicsRectItem();
        rect->setRect(item->RectItem()->rect());
        rect->setData(22, QVariant::fromValue((void*)item));
        rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 55)));
        rect->setPen(QPen(Qt::transparent));
        this->_movegroup->addToGroup(rect);
    }
    this->_pianoroll->Scene()->addItem(this->_movegroup);
}

void PianoRollTool::CleanupEditGroup()
{
    this->_pianoroll->Scene()->removeItem(this->_movegroup);
    delete this->_movegroup;
    this->_movegroup = 0;
}

void PianoRollTool::SetupSelectionRect(PianoRollToolMode::eType mode)
{
    this->_mode = mode;
    this->_selectionRect = new QGraphicsRectItem();
    this->_selectionRect->setRect(this->_startPos.x(), this->_startPos.y(), 1, 1);
    this->_selectionRect->setPen(QPen(QColor::fromRgb(155, 255, 155, 155)));
    this->_selectionRect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 55)));
    this->_pianoroll->Scene()->addItem(this->_selectionRect);
    this->_pianoroll->Selection().Reset();
}

void PianoRollTool::CleanupSelectionRect()
{
    this->_pianoroll->Scene()->removeItem(this->_selectionRect);
    delete this->_selectionRect;
    this->_selectionRect = 0;
}

void PianoRollTool::SetupCreateGroup(PianoRollToolMode::eType mode, const QPointF& pos)
{
    double diffx = pos.x() - (int(pos.x()) % this->_pianoroll->LocalViewScale().horizontal());
    double diffy = pos.y() - (int(pos.y()) % this->_pianoroll->LocalViewScale().vertical());

    this->_mode = mode;
    this->_movegroup = new QGraphicsItemGroup();
    this->_pianoroll->Scene()->addItem(this->_movegroup);
    QGraphicsRectItem* rect = new QGraphicsRectItem();
    rect->setRect(diffx, diffy, this->_pianoroll->LocalViewScale().horizontal(), this->_pianoroll->LocalViewScale().vertical());
    rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 55)));
    rect->setPen(QPen(Qt::transparent));
    rect->setZValue(10);
    this->_movegroup->addToGroup(rect);
}

void PianoRollTool::CleanupCreateGroup()
{
    if (this->_movegroup != 0)
    {
        this->_pianoroll->Scene()->removeItem(this->_movegroup);
        delete this->_movegroup;
        this->_movegroup = 0;
    }
}
