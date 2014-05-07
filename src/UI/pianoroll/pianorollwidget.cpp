#include "pianorollwidget.h"
#include "ui_pianorollwidget.h"
#include "pianorollnoteitem.h"
#include "pianorolltool.h"
#include "trackarea/trackareaclip.h"
#include <QScrollBar>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>
#include <QMouseEvent>
#include <iostream>

using namespace std;

PianoRollWidget::PianoRollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PianoRollWidget)
{
    ui->setupUi(this);

    this->_viewScale.setHorizontalScale(this->ui->horizontalScale->value());

    this->ui->timeline->installEventFilter(this);
    connect(this->ui->notes->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(UpdateScene()));

    this->_scene = new QGraphicsScene();
    this->_scene->installEventFilter(this);
    this->ui->notes->setScene(this->_scene);

    connect(&this->_viewScale, SIGNAL(horizontalScaleChanged(int)), this, SLOT(onViewScaleChanged(int)));
    connect(&this->_viewScale, SIGNAL(verticalScaleChanged(int)), this, SLOT(onViewScaleChanged(int)));

    connect(&Sequencer::Inst().LocalTimeSignature(), SIGNAL(beatUnitChanged(int)), this, SLOT(onViewScaleChanged(int)));
    connect(&Sequencer::Inst().LocalTimeSignature(), SIGNAL(numBeatsChanged(int)), this, SLOT(onViewScaleChanged(int)));

    connect(this->ui->horizontalScale, SIGNAL(valueChanged(int)), &this->_viewScale, SLOT(setHorizontalScale(int)));

    this->_group = new QGraphicsItemGroup();
    this->_group->setData(23, "pianoroll-group");
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_scene->addItem(this->_group);

    connect(&Sequencer::Inst(), SIGNAL(CurrentSongIsUpdated()), this, SLOT(UpdateScene()));

    this->_currentTool = new PianoRollTool(this);
}

PianoRollWidget::~PianoRollWidget()
{
    delete ui;
}

void PianoRollWidget::onViewScaleChanged(int val)
{
    this->UpdateScene();
}

void PianoRollWidget::UpdateScene()
{
    int w = this->_viewScale.horizontal() * 16;
    int h = this->_viewScale.vertical() * NOTES_PER_OCTAVE;

    QPixmap bg(w, h);
    QPainter painter(&bg);
    painter.fillRect(0, 0, w, h, QColor(85, 85, 85));

    painter.setPen(QPen(QColor(46, 46, 46)));
    painter.drawLine(0, 0, w, 0);

    painter.fillRect(0, this->_viewScale.vertical() * 1, w, this->_viewScale.vertical(), QColor(71, 71, 71));
    painter.fillRect(0, this->_viewScale.vertical() * 3, w, this->_viewScale.vertical(), QColor(71, 71, 71));
    painter.fillRect(0, this->_viewScale.vertical() * 5, w, this->_viewScale.vertical(), QColor(71, 71, 71));

    painter.setPen(QPen(QColor(46, 46, 46)));
    painter.drawLine(0, this->_viewScale.vertical() * 7, w, this->_viewScale.vertical() * 7);

    painter.fillRect(0, this->_viewScale.vertical() * 8, w, this->_viewScale.vertical(), QColor(71, 71, 71));
    painter.fillRect(0, this->_viewScale.vertical() * 10, w, this->_viewScale.vertical(), QColor(71, 71, 71));

    for (int i = 0; i < Sequencer::Inst().LocalTimeSignature().numBeats() * 4; i++)
    {
        if (i == 0)
            painter.setPen(QPen(QColor(46, 46, 46)));
        else if (i % Sequencer::Inst().LocalTimeSignature().numBeats() == 0)
            painter.setPen(QPen(QColor(56, 56, 56)));
        else
            painter.setPen(QPen(QColor(63, 63, 63)));

        painter.drawLine(this->_viewScale.horizontal() * i, 0, this->_viewScale.horizontal() * i, h);
    }
    this->ui->notes->setBackgroundBrush(QBrush(bg));

    double width = 1.0;
    for (QList<SequencerClip*>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
    {
        SequencerClip* clip = *i;
        for (QList<SequencerNote*>::iterator i = clip->Notes().begin(); i != clip->Notes().end(); ++i)
        {
            SequencerNote* n = (*i);
            if (n->Start() + n->Length() > width)
                width = n->Start() + n->Length() + 4;

            PianoRollNoteItem* item = this->NoteItemByNote(n);
            if (item == 0)
            {
                item = new PianoRollNoteItem(this, n);
                this->_group->addToGroup(item);
            }
            else
                item->Update();
        }
    }
    this->_scene->setSceneRect(0, 0, width * this->_viewScale.horizontal(), h * NUM_OCTAVES + 1);

    this->ui->notes->update();
    this->ui->timeline->repaint();
}

void PianoRollWidget::AddNoteToCurrentClip(PianoRollNoteItem* item)
{
    this->_group->addToGroup(item);
}

bool PianoRollWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this->_scene)
    {
        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
                this->_currentTool->OnMousePress(e);
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
            {
                this->_currentTool->OnMouseRelease(e);
                Sequencer::Inst().CurrentSongIsUpdated();
            }
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent* e = (QGraphicsSceneMouseEvent*)event;
            if (this->_currentTool != 0)
                this->_currentTool->OnMouseMove(e);
        }
    }
    else if (obj == this->ui->timeline)
    {
        if (event->type() == QEvent::Paint)
        {
            QPainter p(this->ui->timeline);
            p.fillRect(0, 0, this->ui->timeline->width(), this->ui->timeline->height(), QColor(85, 85, 85));

            // Draw timeline
            p.setFont(QFont("Verdana", 10));
            for (int x = 0; (x * this->_viewScale.horizontal()) < this->Scene()->width() + this->ui->timeline->width(); x++)
            {
                if (x == 0)
                    p.setPen(QPen(QColor(46, 46, 46)));
                else if (x % Sequencer::Inst().LocalTimeSignature().numBeats() == 0)
                    p.setPen(QPen(QColor(56, 56, 56)));
                else
                    p.setPen(QPen(QColor(63, 63, 63)));

                if (x % Sequencer::Inst().LocalTimeSignature().numBeats())
                {
                    p.drawLine((x * this->_viewScale.horizontal()) - this->ui->notes->horizontalScrollBar()->value(),
                               this->ui->timeline->height()/2,
                               (x * this->_viewScale.horizontal()) - this->ui->notes->horizontalScrollBar()->value(),
                               this->ui->timeline->height());
                }
                else
                {
                    p.drawLine((x * this->_viewScale.horizontal()) - this->ui->notes->horizontalScrollBar()->value(),
                               0,
                               (x * this->_viewScale.horizontal()) - this->ui->notes->horizontalScrollBar()->value(),
                               this->ui->timeline->height());
                    p.setPen(QPen(QColor(246, 246, 246)));
                    p.drawText((x * this->_viewScale.horizontal()) - this->ui->notes->horizontalScrollBar()->value() + 2,
                               p.fontInfo().pointSize() + 2,
                               QString::number(x+1));
                }
            }
            return true;
        }
    }
    return false;
}

PianoRollNoteItem* PianoRollWidget::NoteItemByNote(SequencerNote* note)
{
    QList<QGraphicsItem*> items = this->_scene->items();
    for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
    {
        PianoRollNoteItem* item = dynamic_cast<PianoRollNoteItem*>(*i);
        if (item != 0 && item->GetNote() == note)
            return item;
    }
    return 0;
}

void PianoRollWidget::ClipSelectionChanged(const QList<TrackAreaClip*>& selection)
{
    this->_selection.Reset();
    while (this->_group->childItems().empty() == false)
    {
        QGraphicsItem* item = this->_group->childItems().back();
        this->_group->removeFromGroup(item);
        delete item;
    }

    this->_clips.clear();
    for (QList<TrackAreaClip*>::const_iterator i = selection.cbegin(); i != selection.cend(); ++i)
        this->_clips.push_back((*i)->GetClip());

    this->UpdateScene();
}

void PianoRollWidget::RemoveNoteItem(PianoRollNoteItem* item)
{
    item->GetNote()->GetClip()->RemoveNote(item->GetNote());
    this->_group->removeFromGroup(item);
}
