#ifndef ALIEN_EFFECTWIDGET_H
#define ALIEN_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Alien_EffectWidget;
}

class MixerEffect;

class Alien_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Alien_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Alien_EffectWidget();

private:
    Ui::Alien_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // ALIEN_EFFECTWIDGET_H
