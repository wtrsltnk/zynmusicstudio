#ifndef TRACKAREA_H
#define TRACKAREA_H

#include <QWidget>
#include <QGraphicsScene>
#include "viewscale.h"
#include "trackproperties.h"
#include "trackareaclip.h"
#include "trackareaselection.h"

namespace Ui {
class TrackAreaWidget;
}

class TrackAreaTool;

class TrackAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackAreaWidget(QWidget *parent = 0);
    virtual ~TrackAreaWidget();

    ViewScale& LocalViewScale() { return this->_viewScale; }
    TrackAreaSelection& Selection() { return this->_selection; }
    QGraphicsScene* Scene() const { return this->_scene; }
    TrackAreaTool* CurrentTool() { return this->_currentTool; }
    void RemoveClip(TrackAreaClip* clip);

    TrackProperties* TrackPropertiesBySequencerTrack(SequencerTrack* track);
    TrackAreaClip* ClipBySequencerClip(SequencerClip* clip);

signals:
    void ActivateClip(TrackAreaClip* clip);

public slots:
    void UpdateTracks();
    void invertSlider(int value);

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::TrackAreaWidget *ui;
    QGraphicsScene* _scene;
    TrackAreaSelection _selection;
    ViewScale _viewScale;
    TrackAreaTool* _currentTool;

    QGraphicsItemGroup* _clips;
    QGraphicsItemGroup* _tracks;

    friend class MainWindow;
    MainWindow* _mainParent;
};

#endif // TRACKAREA_H
