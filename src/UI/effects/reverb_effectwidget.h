#ifndef REVERB_EFFECTWIDGET_H
#define REVERB_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Reverb_EffectWidget;
}

class MixerEffect;

class Reverb_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Reverb_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Reverb_EffectWidget();

private:
    Ui::Reverb_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // REVERB_EFFECTWIDGET_H
