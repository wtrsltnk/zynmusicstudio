#include "eq_effectwidget.h"
#include "ui_eq_effectwidget.h"

EQ_EffectWidget::EQ_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EQ_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

EQ_EffectWidget::~EQ_EffectWidget()
{
    delete ui;
}
