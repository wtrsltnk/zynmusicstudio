#ifndef SEQUENCERAUTOMATEDPROPERTY_H
#define SEQUENCERAUTOMATEDPROPERTY_H

#include <QString>
#include <QList>
#include "sequencerautomatedpropertypoint.h"

class SequencerClip;

class SequencerAutomatedProperty
{
public:
    SequencerAutomatedProperty(SequencerClip* clip);
    virtual ~SequencerAutomatedProperty();

    SequencerClip* GetClip() { return this->_clip; }

    const QString& Title() const { return this->_title; }
    void SetTitle(const QString& title) { this->_title = title; }

    QList<SequencerAutomatedPropertyPoint*>& Points() { return this->_points; }

protected:
    SequencerClip* _clip;
    QString _title;
    QList<SequencerAutomatedPropertyPoint*> _points;
};

#endif // SEQUENCERAUTOMATEDPROPERTY_H
