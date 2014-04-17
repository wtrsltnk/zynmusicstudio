#ifndef TRACKAREATOOL_H
#define TRACKAREATOOL_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPointF>
#include "trackareawidget.h"

namespace TrackAreaToolMode
{
    typedef enum {
        None = 0,
        Select = 1,
        Move = 2,
        Copy = 3,
        Create = 4
    } eType;
}

class TrackAreaTool
{
public:
    TrackAreaTool(TrackAreaWidget* trackarea);
    virtual ~TrackAreaTool();

    virtual void OnMousePress(QGraphicsSceneMouseEvent* e);
    virtual void OnMouseMove(QGraphicsSceneMouseEvent* e);
    virtual void OnMouseRelease(QGraphicsSceneMouseEvent* e);
    virtual void OnMouseDoubleClick(QGraphicsSceneMouseEvent* e);

protected:
    TrackAreaWidget* _trackarea;
    TrackAreaToolMode::eType _mode;

    QGraphicsRectItem* _selectionRect;
    QPointF _startPos;
    QGraphicsItemGroup* _movegroup;
    TrackAreaClip* _createClip;

    void SetupEditGroupFromSelection();
    void CleanupEditGroup();\
    void SetupSelectionRect(TrackAreaToolMode::eType mode);
    void CleanupSelectionRect();
};

#endif // TRACKAREATOOL_H
