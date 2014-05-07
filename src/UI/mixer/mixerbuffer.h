#ifndef MIXERBUFFER_H
#define MIXERBUFFER_H

class MixerBuffer
{
public:
    MixerBuffer();
    MixerBuffer(const MixerBuffer& other);
    virtual ~MixerBuffer();

    float* Left() { return this->_left; }
    float* Right() { return this->_right; }

    void Reset();
    MixerBuffer& operator = (const MixerBuffer& other);
    MixerBuffer& operator += (const MixerBuffer& other);
    MixerBuffer operator + (const MixerBuffer& other);

    void CopyTo(float* l, float* r);
    void CopyFrom(float* l, float* r);

private:
    float* _left;
    float* _right;

};

#endif // MIXERBUFFER_H
