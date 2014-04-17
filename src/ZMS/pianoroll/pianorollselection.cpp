#include "pianorollselection.h"

PianoRollSelection::PianoRollSelection()
{ }

void PianoRollSelection::Reset()
{
    for (QList<PianoRollNoteItem*>::iterator i = this->notes.begin(); i != this->notes.end(); ++i)
        (*i)->Deselect();

    this->notes.clear();
    this->SelectionChanged(this->notes);
}

void PianoRollSelection::StartSelection(PianoRollNoteItem* item)
{
    if (item != 0)
    {
        for (QList<PianoRollNoteItem*>::iterator i = this->notes.begin(); i != this->notes.end(); ++i)
            (*i)->Deselect();

        this->notes.clear();

        item->Select();
        this->notes.push_back(item);
        this->SelectionChanged(this->notes);
    }
}

void PianoRollSelection::AddToSelection(PianoRollNoteItem* item)
{
    if (item != 0 && this->notes.indexOf(item) == -1)
    {
        item->Select();
        this->notes.push_back(item);
        this->SelectionChanged(this->notes);
    }
}

bool PianoRollSelection::IsSelected(PianoRollNoteItem* item)
{
    return (this->notes.indexOf(item) != -1);
}

void PianoRollSelection::ToggleSelection(PianoRollNoteItem* item)
{
    if (this->IsSelected(item) == false)
        this->AddToSelection(item);
    else
        this->RemoveFromSelection(item);
}

void PianoRollSelection::RemoveFromSelection(PianoRollNoteItem* item)
{
    if (this->notes.removeOne(item))
    {
        item->Deselect();
        this->SelectionChanged(this->notes);
    }
}

const QList<PianoRollNoteItem*>& PianoRollSelection::Notes() const
{
    return this->notes;
}
