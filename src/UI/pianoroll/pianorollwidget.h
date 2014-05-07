#ifndef PIANOROLLWIDGET_H
#define PIANOROLLWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <vector>
#include "pianorollselection.h"
#include "viewscale.h"
#include "sequencer/sequencer.h"

namespace Ui {
class PianoRollWidget;
}

class PianoRollTool;
class TrackAreaClip;

#define NOTES_PER_OCTAVE 12
#define NUM_OCTAVES 10

class PianoRollWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PianoRollWidget(QWidget *parent = 0);
    ~PianoRollWidget();

    ViewScale& LocalViewScale() { return this->_viewScale; }
    PianoRollSelection& Selection() { return this->_selection; }
    QGraphicsScene* Scene() const { return this->_scene; }
    PianoRollTool* CurrentTool() { return this->_currentTool; }
    QList<SequencerClip*>& CurrentClip() { return this->_clips; }
    void AddNoteToCurrentClip(PianoRollNoteItem* item);
    void RemoveNoteItem(PianoRollNoteItem* item);

public slots:
    void UpdateScene();
    void onViewScaleChanged(int val);
    void ClipSelectionChanged(const QList<TrackAreaClip*>& selection);

protected:
    virtual bool eventFilter(QObject *, QEvent *);

private:
    Ui::PianoRollWidget *ui;
    QGraphicsScene* _scene;
    PianoRollSelection _selection;
    ViewScale _viewScale;
    PianoRollTool* _currentTool;

    QList<SequencerClip*> _clips;
    QGraphicsItemGroup* _group;
    PianoRollNoteItem* NoteItemByNote(SequencerNote* note);
};

#endif // PIANOROLLWIDGET_H
