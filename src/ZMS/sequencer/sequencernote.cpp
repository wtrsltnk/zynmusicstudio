#include "sequencernote.h"
#include "sequencerclip.h"

SequencerNote::SequencerNote(SequencerClip* clip)
    : _clip(clip), _mute(false), _note(65),
      _velocity(100), _start(0.0), _length(1.0)
{ }

SequencerNote::SequencerNote(SequencerClip* clip, char note, double start, double length)
    : _clip(clip), _mute(false), _note(note),
      _velocity(100), _start(start), _length(length)
{ }

SequencerNote::SequencerNote(const SequencerNote* other)
    : _clip(other->_clip), _mute(other->_mute), _note(other->_note),
      _velocity(other->_velocity), _start(other->_start), _length(other->_length)
{ }
