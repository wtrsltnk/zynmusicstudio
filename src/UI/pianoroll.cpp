/*
  ZynAddSubFX - a software synthesizer

  pianoroll.cpp - User control for editing midi clips
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
#include "pianoroll.h"
#include "ui_pianoroll.h"
#include "pianorollnote.h"
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QScrollBar>
#include "../Sequence/sequence.h"

PianoRoll::PianoRoll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PianoRoll)
{
    ui->setupUi(this);

    this->ui->keyboard->installEventFilter(this);

    this->SetHScale(18);
    this->SetVScale(14);

    this->_scene = new QGraphicsScene();
    this->ui->notes->setScene(this->_scene);
    this->ui->notes->installEventFilter(this);
    this->ui->notes->setSceneRect(0, 0, 1000, 128 * this->VScale());

    QPixmap bg(this->HScale() * 16, this->VScale() * 12);
    QPainter painter(&bg);
    painter.fillRect(0, 0, this->HScale() * 16, this->VScale() * 12, QColor(85, 85, 85));

    painter.setPen(QPen(QColor(46, 46, 46)));
    painter.drawLine(0, 0, this->HScale() * 16, 0);

    painter.fillRect(0, this->VScale() * 1, this->HScale() * 16, this->VScale(), QColor(71, 71, 71));
    painter.fillRect(0, this->VScale() * 3, this->HScale() * 16, this->VScale(), QColor(71, 71, 71));
    painter.fillRect(0, this->VScale() * 5, this->HScale() * 16, this->VScale(), QColor(71, 71, 71));

    painter.setPen(QPen(QColor(46, 46, 46)));
    painter.drawLine(0, this->VScale() * 7, this->HScale() * 16, this->VScale() * 7);

    painter.fillRect(0, this->VScale() * 8, this->HScale() * 16, this->VScale(), QColor(71, 71, 71));
    painter.fillRect(0, this->VScale() * 10, this->HScale() * 16, this->VScale(), QColor(71, 71, 71));

    for (int i = 0; i < 16; i++)
    {
        if (i == 0)
            painter.setPen(QPen(QColor(46, 46, 46)));
        else if (i % 4 == 0)
            painter.setPen(QPen(QColor(56, 56, 56)));
        else
            painter.setPen(QPen(QColor(63, 63, 63)));

        painter.drawLine(i * this->HScale(), 0, i * this->HScale(), this->VScale() * 12);
    }
    this->ui->notes->setBackgroundBrush(QBrush(bg));

    this->_group = new QGraphicsItemGroup();
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_scene->addItem(this->_group);

    connect(this->ui->notes->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnScrollChanged(int)));
}

void PianoRoll::OnScrollChanged(int value)
{
    this->ui->keyboard->update();
}

PianoRoll::~PianoRoll()
{
    delete ui;
}

void PianoRoll::ShowClips(const QList<int>& clips)
{
    this->_clips = clips;
    this->ShowSelectedNotes();
    this->ui->notes->centerOn(
                QPointF(
                    this->_group->boundingRect().x() + this->_group->boundingRect().width() / 2,
                    this->_group->boundingRect().y() + this->_group->boundingRect().height() / 2
                        ));
}

void PianoRoll::UpdateItems()
{
    this->ShowSelectedNotes();
}

void PianoRoll::SelectItem(SnappingGraphicsItem* item)
{
    item->Select();
}

void PianoRoll::ShowSelectedNotes()
{
    while (this->_group->childItems().empty() == false)
    {
        PianoRollNote* note = (PianoRollNote*)this->_group->childItems().back();
        this->_group->removeFromGroup(note);
        delete note;
    }

    for (QList<int>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
    {
        MidiClip* clip = Sequence::getInstance().Pclips[*i];
        if (clip != 0)
        {
            for (std::vector<MidiClip::Note*>::iterator j = clip->Pnotes.begin(); j != clip->Pnotes.end(); ++j)
            {
                MidiClip::Note* note = *j;
                PianoRollNote* n = new PianoRollNote(this, note);
                this->_group->addToGroup(n);
            }
        }
    }
}

bool PianoRoll::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this->ui->keyboard && event->type() == QEvent::Paint)
    {
        QPainter p(this->ui->keyboard);
        p.setPen(QPen(QColor(53, 53, 53)));
        int octavecount = 128 / 12;
        double y = -this->ui->notes->verticalScrollBar()->value();
        double w = this->ui->keyboard->width();

        double a = (7.0 * double(this->VScale())) / 4.0;
        double b = (5.0 * double(this->VScale())) / 3.0;
        for (int o = 0; o < octavecount; o++)
        {
            for (int j = 0; j < 4; j++)
            {
                p.fillRect(QRectF(0.0, double(y) + (double(j) * a), w, a), Qt::white);
                p.drawRect(QRectF(0.0, double(y) + (double(j) * a), w, a));
                if (j != 0 && j != 4)
                {
                    p.fillRect(QRectF(0.0, double(y) + (double(j) * a) - (a * 0.4), w * 0.6, (a * 0.8)), Qt::black);
                    p.drawRect(QRectF(0.0, double(y) + (double(j) * a) - (a * 0.4), w * 0.6, (a * 0.8)));
                }
            }
            y += (a * 4.0);
            for (int j = 0; j < 3; j++)
            {
                p.fillRect(QRectF(0.0, double(y) + (double(j) * b), w, b), Qt::white);
                p.drawRect(QRectF(0.0, double(y) + (double(j) * b), w, b));
                if (j != 0 && j != 3)
                {
                    p.fillRect(QRectF(0.0, double(y) + (double(j) * a) - (a * 0.4), w * 0.6, (a * 0.8)), Qt::black);
                    p.drawRect(QRectF(0.0, double(y) + (double(j) * a) - (a * 0.4), w * 0.6, (a * 0.8)));
                }
            }
            y += (b * 3.0);
        }

        return true;
    }
    if (watched == this->ui->notes && event->type() == QEvent::Scroll)
    {
        this->ui->keyboard->update();
        return true;
    }
    return false;
}
