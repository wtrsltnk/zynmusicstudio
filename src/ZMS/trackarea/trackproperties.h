#ifndef TRACKPROPERTIES_H
#define TRACKPROPERTIES_H

#include <QFrame>
#include "sequencer/sequencertrack.h"

namespace Ui {
class TrackProperties;
}

class TrackProperties : public QFrame
{
    Q_OBJECT

public:
    explicit TrackProperties(SequencerTrack* track);
    ~TrackProperties();

    SequencerTrack* GetTrack() { return this->_track; }

signals:
    void ActivateChannel(Channel* channel);

public slots:
    void UpdateWidget();

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void resizeEvent(QResizeEvent* e);

protected slots:
    void OnCloseClicked();
    void OnChangeChannel();
    void OnChannelUpdated(Channel* channel);

private:
    Ui::TrackProperties *ui;
    SequencerTrack* _track;
};

#endif // TRACKPROPERTIES_H
