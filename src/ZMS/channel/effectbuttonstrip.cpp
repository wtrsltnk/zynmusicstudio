#include "effectbuttonstrip.h"
#include "ui_effectbuttonstrip.h"
#include "mixer/mixer.h"
#include <iostream>

using namespace std;

EffectButton::EffectButton(MixerEffect* effect)
    : _effect(effect)
{
    this->setText("wow");
}

EffectButton::~EffectButton()
{ }

EffectButtonStrip::EffectButtonStrip(MixerChannel* channel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectButtonStrip),
    _channel(channel)
{
    ui->setupUi(this);

    if (this->_channel != 0)
    {
        for (QList<MixerEffect*>::iterator itr = this->_channel->Effects().begin(); itr != this->_channel->Effects().end(); ++itr)
        {
            EffectButton* btn = new EffectButton(*itr);
            // todo: connect clicked()
            this->ui->buttonlayout->insertWidget(this->ui->buttonlayout->count() - 2, btn);
        }
    }

    connect(this->ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnAddEffectClicked()));

    connect(this->_channel, SIGNAL(EffectAdded(MixerEffect*)), this, SLOT(AddEffect(MixerEffect*)));
    connect(this->_channel, SIGNAL(EffectRemoved(MixerEffect*)), this, SLOT(RemoveEffect(MixerEffect*)));
}

EffectButtonStrip::~EffectButtonStrip()
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        EffectButton* btn = dynamic_cast<EffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            this->ui->buttonlayout->removeWidget(btn);
            delete btn;
        }
    }

    delete ui;
}

void EffectButtonStrip::OnAddEffectClicked()
{
    MixerEffect* effect = new MixerEffect();
    this->_channel->AddEffect(effect);
}

void EffectButtonStrip::AddEffect(MixerEffect* effect)
{
    EffectButton* btn = new EffectButton(effect);
    this->ui->buttonlayout->insertWidget(this->ui->buttonlayout->count() - 2, btn);
}

void EffectButtonStrip::RemoveEffect(MixerEffect* effect)
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        EffectButton* btn = dynamic_cast<EffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            if (btn->Effect() == effect)
            {
                this->ui->buttonlayout->removeWidget(btn);
                delete btn;
            }
        }
    }
}
