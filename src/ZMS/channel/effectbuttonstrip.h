#ifndef EFFECTBUTTONSTRIP_H
#define EFFECTBUTTONSTRIP_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include "mixer/mixereffectcontainer.h"

namespace Ui {
class EffectButtonStrip;
}

class EffectButton : public QPushButton
{
    Q_OBJECT
public:
    EffectButton(MixerEffect* effect);
    virtual ~EffectButton();

    MixerEffect* Effect() { return this->_effect; }

private:
    MixerEffect* _effect;

};

class EffectButtonStrip : public QWidget
{
    Q_OBJECT

public:
    EffectButtonStrip(MixerEffectContainer* effects, QWidget *parent = 0);
    ~EffectButtonStrip();

public slots:
    void OnAddEffectClicked();
    void AddEffect(MixerEffect* effect);
    void RemoveEffect(MixerEffect* effect);

private:
    Ui::EffectButtonStrip *ui;
    MixerEffectContainer* _effects;

};

#endif // EFFECTBUTTONSTRIP_H
