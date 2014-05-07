#include "effectbuttonstrip.h"
#include "ui_effectbuttonstrip.h"
#include "mixer/mixer.h"
#include "mixer/mixereffect.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <iostream>

using namespace std;

EffectButton::EffectButton(MixerEffect* effect)
    : _effect(effect)
{
    this->setText(effect->GetName());
    this->setMinimumHeight(24);
}

EffectButton::~EffectButton()
{ }

EffectButtonStrip::EffectButtonStrip(MixerEffectContainer* effects, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectButtonStrip),
    _effects(effects), _removeAction(new QAction("Remove", this)), _editAction(new QAction("Edit", this))
{
    ui->setupUi(this);

    if (this->_effects != 0)
    {
        for (QList<MixerEffect*>::iterator itr = this->_effects->Effects().begin(); itr != this->_effects->Effects().end(); ++itr)
            this->AddEffect(*itr);

        connect(this->_effects, SIGNAL(EffectAdded(MixerEffect*)), this, SLOT(AddEffect(MixerEffect*)));
        connect(this->_effects, SIGNAL(EffectRemoved(MixerEffect*)), this, SLOT(RemoveEffect(MixerEffect*)));
    }

    connect(this->ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnAddEffectClicked()));
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

bool EffectButtonStrip::eventFilter(QObject *o, QEvent *e)
{
    EffectButton* btn = dynamic_cast<EffectButton*>(o);

    if (btn != 0)
    {
        if (e->type() == QEvent::ContextMenu)
        {
            QContextMenuEvent* contextEvent = (QContextMenuEvent*)e;
            QMenu menu;
            connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(ActionSelected(QAction*)));

            this->_removeAction->setData(QVariant::fromValue((void*)btn->Effect()));
            menu.addAction(this->_removeAction);

            this->_editAction->setData(QVariant::fromValue((void*)btn->Effect()));
            menu.addAction(this->_editAction);
            menu.addSeparator();

            menu.exec(contextEvent->globalPos());
            return true;
        }
    }
    return false;
}

void EffectButtonStrip::ActionSelected(QAction* action)
{
    if (action == this->_removeAction)
    {
        QMessageBox dlg;
        dlg.setModal(true);
        dlg.setText("Are you sure to remove this effect?");
        dlg.setIcon(QMessageBox::Information);
        dlg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if (dlg.exec() == QMessageBox::Yes)
        {
            MixerEffect* effect = (MixerEffect*)this->_removeAction->data().value<void*>();
            if (effect != 0)
                this->_effects->RemoveEffect(effect);
        }
    }
    else if (action == this->_editAction)
    {
        // EDIT
    }
}

void EffectButtonStrip::OnAddEffectClicked()
{
    MixerEffect* effect = Mixer::Instance().AddEffect("BLaasdsa");
    this->_effects->AddEffect(effect);
    this->setMinimumHeight(24 * (this->ui->buttonlayout->count() + 2));
}

void EffectButtonStrip::AddEffect(MixerEffect* effect)
{
    EffectButton* btn = new EffectButton(effect);
    btn->installEventFilter(this);
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
