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
    void ActivateChannel(Instrument* channel);

public slots:
    void UpdateWidget();
    void ChannelPicked(QAction* action);
    void ChannelChanged(MixerChannel* channel);
    void TrackChannelNameChanged(const QString& title);

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void resizeEvent(QResizeEvent* e);

protected slots:
    void OnCloseClicked();
    void OnChangeChannel();
    void OnChannelUpdated(MixerChannel* channel);

private:
    Ui::TrackProperties *ui;
    SequencerTrack* _track;

    void PickChannel(const QPoint& pos);
};

#endif // TRACKPROPERTIES_H
