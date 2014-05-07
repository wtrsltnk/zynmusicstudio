#include "effectwidget.h"
#include "ui_effectwidget.h"

EffectWidget::EffectWidget(MixerEffect* effect, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

EffectWidget::~EffectWidget()
{
    delete ui;
}
