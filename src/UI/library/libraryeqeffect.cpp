#include "libraryeqeffect.h"

LibraryEQEffect::LibraryEQEffect(QObject *parent)
    : MixerEffect("EQ", parent)
{ }

LibraryEQEffect::~LibraryEQEffect()
{ }

void LibraryEQEffect::EffectOnBuffer(MixerBuffer& in)
{

}
