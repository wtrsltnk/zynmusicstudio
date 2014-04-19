#include "trackareaclip.h"
#include "trackareawidget.h"
#include "sequencer/sequencer.h"
#include "sequencer/sequencerclip.h"
#include <QPainter>
#include <iostream>

using namespace std;

TrackAreaClip::TrackAreaClip(TrackAreaWidget* trackarea, SequencerClip* clip)
    : _trackarea(trackarea), _clip(clip)
{
    this->_bg = new QGraphicsRectItem(this);
    this->_bg->setBrush(QBrush(this->_clip->Color()));
    this->_bg->setPen(QPen(this->_clip->Color().lighter(50)));
    this->_bg->setData(23, "trackareaclip");
    this->addToGroup(this->_bg);

    this->_notes = new QGraphicsPixmapItem(this);
    this->addToGroup(this->_notes);

    this->handle_r = new QGraphicsRectItem();
    this->addToGroup(this->handle_r);
    this->handle_r->setData(23, "handle");
    this->handle_r->setBrush(QBrush(QColor::fromRgb(0, 0, 0, 1)));
    this->handle_r->setPen(QPen(QColor::fromRgb(0, 0, 0, 1)));
}

void TrackAreaClip::Update(int index)
{
    this->_bg->setRect(this->_clip->Start() * this->_trackarea->LocalViewScale().horizontal(),
                  index * this->_trackarea->LocalViewScale().vertical(),
                  this->_clip->Length() * this->_trackarea->LocalViewScale().horizontal(),
                  this->_trackarea->LocalViewScale().vertical() - 2);
    this->handle_r->setRect(this->_bg->rect().x() + this->_bg->rect().width(),
                  this->_bg->rect().y(),
                  this->_trackarea->LocalViewScale().horizontal(),
                  this->_bg->rect().height());
    QPixmap p = this->GetPixmapFromClip(this->_clip);
    this->_notes->setPixmap(p);
    this->_notes->setPos(this->_clip->Start() * this->_trackarea->LocalViewScale().horizontal(),
                         index * this->_trackarea->LocalViewScale().vertical() + 4);

}

void TrackAreaClip::UpdateFromRect(const QRectF& rect)
{
    this->_clip->SetStart(rect.x() / this->_trackarea->LocalViewScale().horizontal());
    int trackindex = rect.y() / this->_trackarea->LocalViewScale().vertical();
    this->_clip->GetTrack()->GetSong()->SetClipTrack(this->_clip, trackindex);
}

void GetClipSize(QList<SequencerNote*>& notes, double& length, unsigned char& minrange, unsigned char& maxrange)
{
    minrange = 128;
    maxrange = 0;
    length = 0;
    for (QList<SequencerNote*>::iterator i = notes.begin(); i != notes.end(); ++i)
    {
        if (((SequencerNote*)*i)->Note() < minrange) minrange = ((SequencerNote*)*i)->Note();
        if (((SequencerNote*)*i)->Note() > maxrange) maxrange = ((SequencerNote*)*i)->Note();
        if (length < (((SequencerNote*)*i)->Start() + ((SequencerNote*)*i)->Length()))
            length = (((SequencerNote*)*i)->Start() + ((SequencerNote*)*i)->Length());
    }
}

QPixmap TrackAreaClip::GetPixmapFromClip(SequencerClip* clip)
{
    double len = 0;
    unsigned char minrange = 0, maxrange = 0;

    GetClipSize(this->_clip->Notes(), len, minrange, maxrange);

    if (len > 0)
    {
        QPixmap p(len * this->_trackarea->LocalViewScale().horizontal(), this->_trackarea->LocalViewScale().vertical());
        p.fill(QColor::fromRgb(0, 0, 0, 1));

        QPainter painter(&p);
        painter.setPen(QPen(QBrush(QColor::fromRgb(0, 48, 64)), this->_trackarea->LocalViewScale().vertical() / 40));
        for (QList<SequencerNote*>::iterator i = this->_clip->Notes().begin(); i != this->_clip->Notes().end(); ++i)
        {
            SequencerNote* n = *i;
            double percentage = (1.0 / double(maxrange - minrange + 2)) * (double(n->Note() - minrange + 1));
            double y = (this->_trackarea->LocalViewScale().vertical() - 8) * percentage;
            painter.drawLine(n->Start() * this->_trackarea->LocalViewScale().horizontal(), y,
                             (n->Start() + n->Length()) * this->_trackarea->LocalViewScale().horizontal(), y);
        }
        painter.end();
        return p;
    }
    return QPixmap();
}

void TrackAreaClip::Select()
{
    this->_bg->setPen(QPen(Qt::white));
    this->setZValue(5);
}

void TrackAreaClip::Deselect()
{
    this->_bg->setPen(QPen(this->_clip->Color().lighter(50)));
    this->setZValue(1);
}

TrackAreaClip* TrackAreaClip::Clone()
{
    return new TrackAreaClip(this->_trackarea, this->_clip->GetTrack()->CloneClip(this->_clip));
}
