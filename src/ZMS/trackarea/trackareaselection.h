#ifndef TRACKAREASELECTION_H
#define TRACKAREASELECTION_H

#include <QObject>
#include "trackareaclip.h"

class TrackAreaSelection : public QObject
{
    Q_OBJECT

public:
    TrackAreaSelection();
    virtual ~TrackAreaSelection();

    void Reset();
    void StartSelection(TrackAreaClip* item);
    void AddToSelection(TrackAreaClip* item);
    bool IsSelected(TrackAreaClip* item);
    void ToggleSelection(TrackAreaClip* item);
    void RemoveFromSelection(TrackAreaClip* item);
    const QList<TrackAreaClip*>& Clips() const;

signals:
    void SelectionChanged(const QList<TrackAreaClip*>& selection);

private:
    QList<TrackAreaClip*> _clips;
};

#endif // TRACKAREASELECTION_H
