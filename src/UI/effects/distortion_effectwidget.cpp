#include "distortion_effectwidget.h"
#include "ui_distortion_effectwidget.h"

Distortion_EffectWidget::Distortion_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Distortion_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Distortion_EffectWidget::~Distortion_EffectWidget()
{
    delete ui;
}
