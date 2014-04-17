#ifndef PIANOROLLNOTEITEM_H
#define PIANOROLLNOTEITEM_H

#include <QGraphicsItemGroup>

class SequencerNote;
class PianoRollWidget;

class PianoRollNoteItem : public QGraphicsItemGroup
{
public:
    PianoRollNoteItem(PianoRollWidget* piano, SequencerNote* note);

    void Update();
    void UpdateFromRect(QRectF& rect);
    SequencerNote* GetNote() { return this->note; }
    void Select();
    void Deselect();
    PianoRollNoteItem* Clone();

    const QGraphicsRectItem* RectItem() const { return this->item; }
private:
    PianoRollWidget* pianoroll;
    SequencerNote* note;
    QGraphicsRectItem* item;
    QGraphicsRectItem* handle_l;
    QGraphicsRectItem* handle_r;
};

#endif // PIANOROLLNOTEITEM_H
