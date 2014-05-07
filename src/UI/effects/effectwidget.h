#ifndef EFFECTWIDGET_H
#define EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class EffectWidget;
}

class MixerEffect;

class EffectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    ~EffectWidget();

    MixerEffect* Effect() { return this->_effect; }

private:
    Ui::EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // EFFECTWIDGET_H
