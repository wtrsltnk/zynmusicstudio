#include "echo_effectwidget.h"
#include "ui_echo_effectwidget.h"

Echo_EffectWidget::Echo_EffectWidget(MixerEffect* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Echo_EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);
}

Echo_EffectWidget::~Echo_EffectWidget()
{
    delete ui;
}
