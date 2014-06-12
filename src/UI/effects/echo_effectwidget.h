#ifndef ECHO_EFFECTWIDGET_H
#define ECHO_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Echo_EffectWidget;
}

class MixerEffect;

class Echo_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Echo_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Echo_EffectWidget();

private:
    Ui::Echo_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // ECHO_EFFECTWIDGET_H
