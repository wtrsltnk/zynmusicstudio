#include "effectwidget.h"
#include "ui_effectwidget.h"
#include "../mixer/mixereffect.h"
#include "../effects/alien_effectwidget.h"
#include "../effects/chorus_effectwidget.h"
#include "../effects/distortion_effectwidget.h"
#include "../effects/dynamic_effectwidget.h"
#include "../effects/echo_effectwidget.h"
#include "../effects/eq_effectwidget.h"
#include "../effects/phaser_effectwidget.h"
#include "../effects/reverb_effectwidget.h"
#include <QVBoxLayout>

EffectWidget::EffectWidget(MixerEffect* effect, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::EffectWidget),
    _effect(effect)
{
    ui->setupUi(this);

    this->ui->cmbEffectSelector->addItem("None");
    this->ui->cmbEffectSelector->addItem("Alien");
    this->ui->cmbEffectSelector->addItem("Chorus");
    this->ui->cmbEffectSelector->addItem("Distortion");
    this->ui->cmbEffectSelector->addItem("Dynamic filter");
    this->ui->cmbEffectSelector->addItem("Echo");
    this->ui->cmbEffectSelector->addItem("EQ");
    this->ui->cmbEffectSelector->addItem("Phaser");
    this->ui->cmbEffectSelector->addItem("Reverse");

    connect(this->ui->cmbEffectSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEffectTypeChanged(int)));
}

EffectWidget::~EffectWidget()
{
    delete ui;
}

void EffectWidget::OnEffectTypeChanged(int index)
{
    if (this->ui->effectWidgetLayout->layout()->count() > 1)
    {
        QLayoutItem* item = this->ui->effectWidgetLayout->layout()->itemAt(this->ui->effectWidgetLayout->layout()->count() - 1);
        QWidget* widget = item->widget();
        this->ui->effectWidgetLayout->layout()->removeWidget(widget);
        delete widget;
    }

    this->_effect->SetEffect(index);
    QWidget* w = 0;
    if (index == EffectTypes::Alien)
    {
        w = new Alien_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Chorus)
    {
        w = new Chorus_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Distortion)
    {
        w = new Distortion_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Dynamic)
    {
        w = new Dynamic_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Echo)
    {
        w = new Echo_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::EQ)
    {
        w = new EQ_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Phaser)
    {
        w = new Phaser_EffectWidget(this->_effect, this);
    }
    else if (index == EffectTypes::Reverb)
    {
        w = new Reverb_EffectWidget(this->_effect, this);
    }
    else
    {
        w = new QLabel("No effect selected");
    }
    this->ui->effectWidgetLayout->layout()->addWidget(w);
    ((QVBoxLayout*)this->ui->effectWidgetLayout->layout())->setAlignment(w, Qt::AlignTop);
}
