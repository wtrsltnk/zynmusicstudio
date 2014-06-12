#include "chorus_effectwidget.h"
#include "ui_chorus_effectwidget.h"

Chorus_EffectWidget::Chorus_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chorus_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Chorus_EffectWidget::~Chorus_EffectWidget()
{
    delete ui;
}
