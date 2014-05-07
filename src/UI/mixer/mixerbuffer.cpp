#include "mixerbuffer.h"
#include "../../globals.h"

MixerBuffer::MixerBuffer()
    : _left(new float[synth->buffersize]),
      _right(new float[synth->buffersize])
{ }

MixerBuffer::MixerBuffer(const MixerBuffer& other)
    : _left(new float[synth->buffersize]),
      _right(new float[synth->buffersize])
{
    (*this) = other;
}

MixerBuffer::~MixerBuffer()
{
    delete []this->_left;
    this->_left = 0;
    delete []this->_right;
    this->_right = 0;
}

void MixerBuffer::Reset()
{
    for (int i = 0; i < synth->buffersize; i++)
        this->_left[i] = this->_right[i] = 0;
}

MixerBuffer& MixerBuffer::operator = (const MixerBuffer& other)
{
    for (int i = 0; i < synth->buffersize; i++)
    {
        this->_left[i] = other._left[i];
        this->_right[i] = other._right[i];
    }
    return (*this);
}

MixerBuffer& MixerBuffer::operator += (const MixerBuffer& other)
{
    for (int i = 0; i < synth->buffersize; i++)
    {
        this->_left[i] += other._left[i];
        this->_right[i] += other._right[i];
    }
    return (*this);
}

MixerBuffer MixerBuffer::operator + (const MixerBuffer& other)
{
    MixerBuffer result;

    for (int i = 0; i < synth->buffersize; i++)
    {
        result._left[i] = this->_left[i] + other._left[i];
        result._right[i] = this->_right[i] + other._right[i];
    }

    return result;
}

void MixerBuffer::CopyTo(float* l, float* r)
{
    for (int i = 0; i < synth->buffersize; i++)
    {
        l[i] = this->_left[i];
        r[i] = this->_right[i];
    }
}

void MixerBuffer::CopyFrom(float* l, float* r)
{
    for (int i = 0; i < synth->buffersize; i++)
    {
        this->_left[i] = l[i];
        this->_right[i] = r[i];
    }
}
