#ifndef TRACKAREACLIP_H
#define TRACKAREACLIP_H

#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class TrackAreaWidget;
class SequencerClip;

class TrackAreaClip : public QGraphicsItemGroup
{
public:
    TrackAreaClip(TrackAreaWidget* trackarea, SequencerClip* clip);

    void Update(int index);
    void UpdateFromRect(const QRectF& rect);

    void Select();
    void Deselect();
    TrackAreaClip* Clone();
    SequencerClip* GetClip() { return this->_clip; }

    const QGraphicsRectItem* RectItem() const { return this->_bg; }
private:
    TrackAreaWidget* _trackarea;
    QGraphicsRectItem* _bg;
    QGraphicsPixmapItem* _notes;
    QGraphicsRectItem* handle_r;
    SequencerClip* _clip;

    QPixmap GetPixmapFromClip(SequencerClip* clip);
};

#endif // TRACKAREACLIP_H
