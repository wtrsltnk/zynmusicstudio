#ifndef EQ_EFFECTWIDGET_H
#define EQ_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class EQ_EffectWidget;
}

class MixerEffect;

class EQ_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    EQ_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~EQ_EffectWidget();

private:
    Ui::EQ_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // EQ_EFFECTWIDGET_H
