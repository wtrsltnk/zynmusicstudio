#include "phaser_effectwidget.h"
#include "ui_phaser_effectwidget.h"

Phaser_EffectWidget::Phaser_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Phaser_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Phaser_EffectWidget::~Phaser_EffectWidget()
{
    delete ui;
}
