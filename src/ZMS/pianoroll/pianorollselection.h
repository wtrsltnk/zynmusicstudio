#ifndef PIANOROLLSELECTION_H
#define PIANOROLLSELECTION_H

#include <QObject>
#include "pianorollnoteitem.h"
#include <QList>

class PianoRollSelection : public QObject
{
    Q_OBJECT

public:
    PianoRollSelection();

    void Reset();
    void StartSelection(PianoRollNoteItem* item);
    void AddToSelection(PianoRollNoteItem* item);
    bool IsSelected(PianoRollNoteItem* item);
    void ToggleSelection(PianoRollNoteItem* item);
    void RemoveFromSelection(PianoRollNoteItem* item);
    const QList<PianoRollNoteItem*>& Notes() const;

signals:
    void SelectionChanged(const QList<PianoRollNoteItem*>& selection);

private:
    QList<PianoRollNoteItem*> notes;
};

#endif // PIANOROLLSELECTION_H
