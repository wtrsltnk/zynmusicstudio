#include "alien_effectwidget.h"
#include "ui_alien_effectwidget.h"

Alien_EffectWidget::Alien_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Alien_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Alien_EffectWidget::~Alien_EffectWidget()
{
    delete ui;
}
