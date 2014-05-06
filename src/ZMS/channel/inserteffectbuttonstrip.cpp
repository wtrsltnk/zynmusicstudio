#include "inserteffectbuttonstrip.h"
#include "ui_inserteffectbuttonstrip.h"
#include "mixer/mixereffect.h"
#include "mixer/mixer.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <iostream>

using namespace std;

InsertEffectButton::InsertEffectButton(MixerInsertEffect *effect, QWidget* parent)
    : QWidget(parent), _effect(effect)
{
    this->ui_layout = new QHBoxLayout(this);
    this->ui_layout->setSpacing(0);
    this->ui_layout->setMargin(0);
    this->setLayout(this->ui_layout);

    this->ui_button = new QPushButton(this);
    this->ui_button->setText("test");
    this->ui_button->setMaximumHeight(24);
    this->ui_layout->addWidget(this->ui_button);

    this->ui_dial = new QDial(this);
    this->ui_dial->setMaximumWidth(24);
    this->ui_dial->setMaximumHeight(24);
    this->ui_layout->addWidget(this->ui_dial);

    connect(this, SIGNAL(setText(QString)), this->ui_button, SLOT(setWindowTitle(QString)));

    this->ui_button->installEventFilter(this);
    this->ui_dial->installEventFilter(this);
}

bool InsertEffectButton::eventFilter(QObject *o, QEvent *e)
{
    return false;
}

InsertEffectButton::~InsertEffectButton()
{
    delete this->ui_button;
    delete this->ui_dial;
}

InsertEffectButtonStrip::InsertEffectButtonStrip(MixerInsertEffectContainer* effects, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertEffectButtonStrip),
    _effects(effects), _removeAction(new QAction("Remove", this)), _editAction(new QAction("Edit", this))
{
    ui->setupUi(this);

    if (effects != 0)
    {
        for (QList<MixerInsertEffect*>::iterator itr = effects->InsertEffects().begin(); itr != effects->InsertEffects().end(); ++itr)
            this->AddEffect(*itr);

        connect(this->_effects, SIGNAL(InsertEffectAdded(MixerInsertEffect*)), this, SLOT(AddEffect(MixerInsertEffect*)));
        connect(this->_effects, SIGNAL(InsertEffectRemoved(MixerInsertEffect*)), this, SLOT(RemoveEffect(MixerInsertEffect*)));
    }

    connect(this->ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnAddEffectClicked()));
}

InsertEffectButtonStrip::~InsertEffectButtonStrip()
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        InsertEffectButton* btn = dynamic_cast<InsertEffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            this->ui->buttonlayout->removeWidget(btn);
            delete btn;
        }
    }

    delete ui;
}

bool InsertEffectButtonStrip::eventFilter(QObject *o, QEvent *e)
{
    QPushButton* btn = dynamic_cast<QPushButton*>(o);
    QDial* dial = dynamic_cast<QDial*>(o);
    InsertEffectButton* effectbtn = dynamic_cast<InsertEffectButton*>(o);

    if (effectbtn == 0)
    {
        if (btn != 0)
            effectbtn = dynamic_cast<InsertEffectButton*>(btn->parent());
        else if (dial != 0)
            effectbtn = dynamic_cast<InsertEffectButton*>(dial->parent());
    }

    if (effectbtn != 0)
    {
        if (e->type() == QEvent::ContextMenu)
        {
            QContextMenuEvent* contextEvent = (QContextMenuEvent*)e;
            QMenu menu;
            connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(ActionSelected(QAction*)));

            this->_removeAction->setData(QVariant::fromValue((void*)effectbtn->Effect()));
            menu.addAction(this->_removeAction);

            this->_editAction->setData(QVariant::fromValue((void*)effectbtn->Effect()));
            menu.addAction(this->_editAction);
            menu.addSeparator();

            menu.exec(contextEvent->globalPos());
            return true;
        }
    }
    return false;
}

void InsertEffectButtonStrip::ActionSelected(QAction* action)
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
            MixerInsertEffect* effect = (MixerInsertEffect*)this->_removeAction->data().value<void*>();
            if (effect != 0)
            {
                this->_effects->RemoveInsertEffect(effect);
            }
        }
    }
    else if (action == this->_editAction)
    {
        // EDIT
    }
}

void InsertEffectButtonStrip::OnAddEffectClicked()
{
    // Todo : pick an insert effect here instead of create new one
    MixerEffect* effect = Mixer::Instance().AddInsertEffect("Insert effect");
    this->_effects->AddInsertEffect(new MixerInsertEffect(effect));
    this->setMinimumHeight(24 * (this->ui->buttonlayout->count() + 2));
}

void InsertEffectButtonStrip::AddEffect(MixerInsertEffect* effect)
{
    InsertEffectButton* btn = new InsertEffectButton(effect);
    btn->setText(effect->GetEffect()->GetName());
    btn->installEventFilter(this);
    this->ui->buttonlayout->insertWidget(this->ui->buttonlayout->count() - 2, btn);
}

void InsertEffectButtonStrip::RemoveEffect(MixerInsertEffect* effect)
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        InsertEffectButton* btn = dynamic_cast<InsertEffectButton*>(this->ui->buttonlayout->itemAt(i)->widget());
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
