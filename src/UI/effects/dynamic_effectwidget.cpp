#include "dynamic_effectwidget.h"
#include "ui_dynamic_effectwidget.h"
#include "./mixer/mixereffect.h"

Dynamic_EffectWidget::Dynamic_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dynamic_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Dynamic_EffectWidget::~Dynamic_EffectWidget()
{
    delete ui;
}
