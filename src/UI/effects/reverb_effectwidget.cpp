#include "reverb_effectwidget.h"
#include "ui_reverb_effectwidget.h"

Reverb_EffectWidget::Reverb_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reverb_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Reverb_EffectWidget::~Reverb_EffectWidget()
{
    delete ui;
}
