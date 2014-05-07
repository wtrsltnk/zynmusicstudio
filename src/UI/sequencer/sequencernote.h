#ifndef NOTE_H
#define NOTE_H

class SequencerClip;

class SequencerNote
{
public:
    SequencerNote(SequencerClip* clip);
    SequencerNote(SequencerClip* clip, char note, double start, double length);
    SequencerNote(const SequencerNote* other);

    SequencerClip* GetClip() { return this->_clip; }

    bool IsMute() { return this->_mute; }
    void SetMute(bool mute) { this->_mute = mute; }

    char Note() { return this->_note; }
    void SetNote(char note) { this->_note = note; }

    char Velocity() { return this->_velocity; }
    void SetVelocity(char velocity) { this->_velocity = velocity; }

    double Start() { return this->_start; }
    void SetStart(double start) { this->_start = start; }

    double Length() { return this->_length; }
    void SetLength(double len) { this->_length = len; }

private:
    SequencerClip* _clip;

    bool _mute;
    char _note;
    char _velocity;
    double _start;
    double _length;
};

#endif // NOTE_H
