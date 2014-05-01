#ifndef EFFECTBUTTONSTRIP_H
#define EFFECTBUTTONSTRIP_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include "mixer/mixereffect.h"

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
    EffectButtonStrip(MixerChannel* channel, QWidget *parent = 0);
    ~EffectButtonStrip();

    void UpdateMinHeight();

public slots:
    void OnAddEffectClicked();
    void AddEffect(MixerEffect* effect);
    void RemoveEffect(MixerEffect* effect);

private:
    Ui::EffectButtonStrip *ui;
    MixerChannel* _channel;

};

#endif // EFFECTBUTTONSTRIP_H
