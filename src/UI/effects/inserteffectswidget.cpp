#include "inserteffectswidget.h"
#include "ui_inserteffectswidget.h"
#include "effectwidget.h"
#include "mixer/mixer.h"
#include "mixer/mixereffect.h"
#include <iostream>

using namespace std;

InsertEffectsWidget::InsertEffectsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertEffectsWidget)
{
    ui->setupUi(this);

    for(QList<MixerEffect*>::iterator itr = Mixer::Instance().InsertEffects().begin(); itr != Mixer::Instance().InsertEffects().end(); ++itr)
        this->AddInsertEffect(*itr);

    connect(this->ui->btnNew, SIGNAL(clicked()), this, SLOT(OnNewEffectClicked()));
    connect(&Mixer::Instance(), SIGNAL(InsertEffectAdded(MixerEffect*)), this, SLOT(AddInsertEffect(MixerEffect*)));
    connect(&Mixer::Instance(), SIGNAL(InsertEffectRemoved(MixerEffect*)), this, SLOT(RemoveInsertEffect(MixerEffect*)));
    connect(this->ui->tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(OnCloseEffectClicked(int)));
}

InsertEffectsWidget::~InsertEffectsWidget()
{
    delete ui;
}

void InsertEffectsWidget::OnNewEffectClicked()
{
    Mixer::Instance().AddInsertEffect("wow");
}

void InsertEffectsWidget::OnCloseEffectClicked(int index)
{
    EffectWidget* widget = dynamic_cast<EffectWidget*>(this->ui->tabs->widget(index));
    Mixer::Instance().RemoveInsertEffect(widget->Effect());
}

void InsertEffectsWidget::AddInsertEffect(MixerEffect* effect)
{
    EffectWidget* w = new EffectWidget(effect, this);
    this->ui->tabs->addTab(w, effect->GetName());
}

void InsertEffectsWidget::RemoveInsertEffect(MixerEffect* effect)
{
    for (int i = 0; i < this->ui->tabs->count(); i++)
    {
        EffectWidget* widget = dynamic_cast<EffectWidget*>(this->ui->tabs->widget(i));
        if (widget != 0 && widget->Effect() == effect)
        {
            this->ui->tabs->removeTab(i);
            delete widget;
            break;
        }
    }
}

void InsertEffectsWidget::SelectEffect(MixerEffect* effect)
{
    for (int i = 0; i < this->ui->tabs->count(); i++)
    {
        EffectWidget* widget = dynamic_cast<EffectWidget*>(this->ui->tabs->widget(i));
        if (widget != 0 && widget->Effect() == effect)
        {
            this->ui->tabs->setCurrentIndex(i);
            break;
        }
    }
}
