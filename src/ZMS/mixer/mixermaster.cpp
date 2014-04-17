#include "mixermaster.h"
#include "./Nio/MidiIn.h"
#include "./Nio/AudioOut.h"

using namespace std;

MixerMaster::MixerMaster(QObject *parent) :
    QObject(parent)
{ }
