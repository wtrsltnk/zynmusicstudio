#ifndef DISTORTION_EFFECTWIDGET_H
#define DISTORTION_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Distortion_EffectWidget;
}

class MixerEffect;

class Distortion_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Distortion_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Distortion_EffectWidget();

private:
    Ui::Distortion_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // DISTORTION_EFFECTWIDGET_H
