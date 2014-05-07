/*
  ZynAddSubFX - a software synthesizer

  pianoroll.h - User control for editing midi clips
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
#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QWidget>
#include <QGraphicsScene>
#include "snappingcontainer.h"
#include "../Sequence/midiclip.h"

namespace Ui {
class PianoRoll;
}

class PianoRoll : public QWidget, public SnappingContainer
{
    Q_OBJECT

public:
    explicit PianoRoll(QWidget *parent = 0);
    virtual ~PianoRoll();

    void ShowClips(const QList<int>& clips);

    virtual void UpdateItems();
    virtual void SelectItem(SnappingGraphicsItem* item);

protected slots:
    void OnScrollChanged(int value);

private:
    Ui::PianoRoll *ui;
    QList<int> _clips;

    void ShowSelectedNotes();
    virtual bool eventFilter(QObject* watched, QEvent* event);
};

#endif // PIANOROLL_H
