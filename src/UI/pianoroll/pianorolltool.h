#ifndef PIANOROLLTOOL_H
#define PIANOROLLTOOL_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPointF>
#include "pianorollwidget.h"

namespace PianoRollToolMode
{
    typedef enum {
        None = 0,
        Select = 1,
        Move = 2,
        Size = 3,
        Copy = 4,
        Create = 5
    } eType;
}

class PianoRollTool
{
public:
    PianoRollTool(PianoRollWidget* pianoroll);
    virtual ~PianoRollTool();

    virtual void OnMousePress(QGraphicsSceneMouseEvent* e);
    virtual void OnMouseMove(QGraphicsSceneMouseEvent* e);
    virtual void OnMouseRelease(QGraphicsSceneMouseEvent* e);

protected:
    PianoRollWidget* _pianoroll;
    QList<SequencerNote> _initialNotes;
    PianoRollToolMode::eType _mode;

    QGraphicsRectItem* _selectionRect;
    QPointF _startPos;
    QGraphicsItemGroup* _movegroup;

    void SetupEditGroupFromSelection(PianoRollToolMode::eType mode);
    void CleanupEditGroup();\
    void SetupSelectionRect(PianoRollToolMode::eType mode);
    void CleanupSelectionRect();
    void SetupCreateGroup(PianoRollToolMode::eType mode, const QPointF& pos);
    void CleanupCreateGroup();
};

#endif // PIANOROLLTOOL_H
