#include "mixergenerator.h"
#include "mixersource.h"

MixerGenerator::MixerGenerator(QObject* parent)
    : QObject(parent), _source(0)
{ }

MixerGenerator::~MixerGenerator()
{
    this->SetSource(0);
}

void MixerGenerator::SetSource(MixerSource* source)
{
    if (this->_source != source)
    {
        if (this->_source != 0)
            this->_source->SetGenerator(0);

        this->_source = source;
        if (this->_source != 0)
            this->_source->SetGenerator(this);

        emit SourceChanged(this->_source);
    }
}
