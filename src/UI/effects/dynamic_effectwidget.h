#ifndef DYNAMIC_EFFECTWIDGET_H
#define DYNAMIC_EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class Dynamic_EffectWidget;
}

class MixerEffect;

class Dynamic_EffectWidget : public QWidget
{
    Q_OBJECT

public:
    Dynamic_EffectWidget(MixerEffect* effect, QWidget *parent = 0);
    virtual ~Dynamic_EffectWidget();

private:
    Ui::Dynamic_EffectWidget *ui;
    MixerEffect* _effect;
};

#endif // DYNAMIC_EFFECTWIDGET_H
