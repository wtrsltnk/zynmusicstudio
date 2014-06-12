#ifndef CHORUS_EFFECTWIDGET_H
#define CHORUS_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Chorus_EffectWidget;
}

class MixerEffect;

class Chorus_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Chorus_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Chorus_EffectWidget();

private:
    Ui::Chorus_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // CHORUS_EFFECTWIDGET_H
