#include "pianorollnoteitem.h"
#include "pianorollwidget.h"
#include "pianorolltool.h"
#include <QGraphicsSceneMouseEvent>
#include <iostream>

using namespace std;

PianoRollNoteItem::PianoRollNoteItem(PianoRollWidget* piano, SequencerNote* n)
    : QGraphicsItemGroup(0), pianoroll(piano), note(n)
{
    this->setData(23, "group");
    this->setZValue(1);

    this->item = new QGraphicsRectItem();
    this->addToGroup(this->item);
    this->item->setData(23, "note");
    this->item->setBrush(QBrush(this->note->GetClip()->Color()));
    this->item->setPen(QPen(this->note->GetClip()->Color().lighter(50)));
    this->item->setCursor(Qt::SizeAllCursor);

    this->handle_r = new QGraphicsRectItem();
    this->addToGroup(this->handle_r);
    this->handle_r->setData(23, "handle");
    this->handle_r->setBrush(QBrush(QColor::fromRgb(0, 0, 0, 1)));
    this->handle_r->setPen(QPen(QColor::fromRgb(0, 0, 0, 1)));
    this->handle_r->setCursor(Qt::SizeHorCursor);

    this->Update();
}

void PianoRollNoteItem::Update()
{
    this->item->setRect(this->note->Start() * this->pianoroll->LocalViewScale().horizontal(),
                  this->note->Note() * this->pianoroll->LocalViewScale().vertical(),
                  this->note->Length() * this->pianoroll->LocalViewScale().horizontal(),
                  this->pianoroll->LocalViewScale().vertical());
    this->handle_r->setRect((this->note->Start() + this->note->Length()) * this->pianoroll->LocalViewScale().horizontal(),
                    this->note->Note() * this->pianoroll->LocalViewScale().vertical(),
                    this->pianoroll->LocalViewScale().horizontal(),
                    this->pianoroll->LocalViewScale().vertical());
}

void PianoRollNoteItem::UpdateFromRect(const QRectF& rect)
{
    this->note->SetStart(double(rect.x()) / double(this->pianoroll->LocalViewScale().horizontal()));
    this->note->SetNote(rect.y() / this->pianoroll->LocalViewScale().vertical());
    this->note->SetLength(rect.width() / this->pianoroll->LocalViewScale().horizontal());
    this->Update();
}

void PianoRollNoteItem::Select()
{
    this->item->setPen(QPen(Qt::white));
    this->setZValue(5);
}

void PianoRollNoteItem::Deselect()
{
    this->item->setPen(QPen(this->note->GetClip()->Color().lighter(50)));
    this->setZValue(1);
}

PianoRollNoteItem* PianoRollNoteItem::Clone()
{
    return new PianoRollNoteItem(this->pianoroll, this->note->GetClip()->CloneNote(this->note));
}
