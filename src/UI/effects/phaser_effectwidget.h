#ifndef PHASER_EFFECTWIDGET_H
#define PHASER_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Phaser_EffectWidget;
}

class MixerEffect;

class Phaser_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Phaser_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Phaser_EffectWidget();

private:
    Ui::Phaser_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // PHASER_EFFECTWIDGET_H
