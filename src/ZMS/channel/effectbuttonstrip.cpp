#include "effectbuttonstrip.h"
#include "ui_effectbuttonstrip.h"
#include <iostream>

using namespace std;

EffectButton::EffectButton(EffectMgr* effect)
    : _effect(effect)
{ }

EffectButton::~EffectButton()
{ }

EffectButtonStrip::EffectButtonStrip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectButtonStrip)
{
    ui->setupUi(this);

    connect(this->ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnAddEffectClicked()));
}

EffectButtonStrip::~EffectButtonStrip()
{
    delete ui;
}

void EffectButtonStrip::OnUpdateEffectButtons(QList<EffectMgr*>& effects)
{
    // Remove buttons of removed effects
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        EffectButton* btn = dynamic_cast<EffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            if (effects.contains(btn->Effect()) == false)
            {
                this->ui->buttonlayout->removeWidget(btn);
                delete btn;
                i--;
            }
        }
    }

    // Add new effects were needed
    for (QList<EffectMgr*>::iterator i = effects.begin(); i != effects.end(); ++i)
    {
        EffectButton* btn = this->GetButtonByEffect(*i);
        if (btn == 0)
        {
            btn = new EffectButton(*i);
            this->ui->buttonlayout->insertWidget(this->ui->buttonlayout->count() - 2, btn);
            connect(btn, SIGNAL(clicked()), this, SLOT(OnEffectClicked()));
        }
        btn->setText(QString((*i)->getEffectName()));
    }

    this->UpdateMinHeight();
}

void EffectButtonStrip::UpdateMinHeight()
{
    int min = this->ui->buttonlayout->count();
    min += 5 - (min % 5);
    this->setMinimumHeight(min * this->ui->btnAdd->height());
    this->setMaximumHeight(min * this->ui->btnAdd->height());
}

EffectButton* EffectButtonStrip::GetButtonByEffect(EffectMgr* effect)
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        EffectButton* btn = dynamic_cast<EffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
            if (btn->Effect() == effect)
                return btn;
    }
    return 0;
}

void EffectButtonStrip::OnEffectClicked()
{ }

void EffectButtonStrip::OnAddEffectClicked()
{
    QList<EffectMgr*> effects;
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        EffectButton* btn = dynamic_cast<EffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
            effects.append(btn->Effect());
    }
    effects << new EffectMgr(true, 0);
    this->OnUpdateEffectButtons(effects);
}
