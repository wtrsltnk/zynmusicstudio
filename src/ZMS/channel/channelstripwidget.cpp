#include "channelstripwidget.h"
#include "ui_channelstripwidget.h"
#include "sendbuttonstrip.h"
#include "effectbuttonstrip.h"
#include "inserteffectbuttonstrip.h"
#include "sequencer/sequencer.h"
#include "mixer/mixer.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QMenu>
#include <iostream>

using namespace std;

ChannelStripWidget::ChannelStripWidget(MixerChannel* channel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ChannelStripWidget),
    _channel(channel)
{
    ui->setupUi(this);

    this->ui_effectButtonStrip = new EffectButtonStrip(&channel->Effects, this);
    this->ui->channelParts->insertWidget(2, this->ui_effectButtonStrip);

    this->ui_insertEffectButtonStrip = new InsertEffectButtonStrip(&channel->InsertEffects, this);
    this->ui->channelParts->insertWidget(3, this->ui_insertEffectButtonStrip);

    this->ui_sendButtonStrip = new SendButtonStrip(&channel->SendSource, this);
    this->ui->channelParts->insertWidget(4, this->ui_sendButtonStrip);

    this->ui->lblName->installEventFilter(this);
    this->ui->btnEdit->installEventFilter(this);

    connect(this->_channel, SIGNAL(VolumeChanged(int)), this->ui->volume, SLOT(setValue(int)));
    connect(this->ui->volume, SIGNAL(valueChanged(int)), this->_channel, SLOT(SetVolume(int)));
    connect(this->_channel, SIGNAL(NameChanged(QString)), this->ui->lblName, SLOT(setText(QString)));
    connect(this->_channel, SIGNAL(ColorChanged(QColor)), this, SLOT(SetChannelColor(QColor)));
    connect(this->_channel, SIGNAL(ChannelInputChanged(MixerChannelInput*)), this, SLOT(ChangeChannelInput(MixerChannelInput*)));

    this->ui->volume->setValue(this->_channel->GetVolume());
    this->ui->lblName->setText(this->_channel->GetName());
    this->SetChannelColor(this->_channel->GetColor());
    this->ChangeChannelInput(this->_channel->ChannelInput());

    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnEdit, SIGNAL(clicked()), this, SLOT(OnInstrumentClicked()));
}

ChannelStripWidget::~ChannelStripWidget()
{
    delete this->ui_sendButtonStrip;
    delete this->ui_effectButtonStrip;
    delete ui;
}

void ChannelStripWidget::OnCloseClicked()
{
    QMessageBox dlg;
    dlg.setModal(true);
    dlg.setText("Are you sure to remove this track?");
    dlg.setIcon(QMessageBox::Information);
    dlg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (dlg.exec() == QMessageBox::Yes)
    {
        Mixer::Instance().RemoveChannel(this->_channel);
        this->_channel = 0;
    }
}

bool ChannelStripWidget::eventFilter(QObject* o, QEvent* e)
{
    if (o == this->ui->lblName && e->type() == QEvent::MouseButtonDblClick)
    {
        QInputDialog dlg;
        dlg.setLabelText("Enter a new title:");
        dlg.setModal(true);
        dlg.setTextValue(this->ui->lblName->text());
        if (dlg.exec() == QDialog::Accepted)
        {
            this->_channel->SetName(dlg.textValue());
        }
        return true;
    }

    if (o == this->ui->btnEdit && e->type() == QEvent::ContextMenu)
    {
        QContextMenuEvent* contextEvent = (QContextMenuEvent*)e;
        this->PickInstrument(contextEvent->globalPos());
        return true;
    }

    return false;
}

void ChannelStripWidget::SetChannelColor(const QColor& color)
{
    this->ui->lblName->setStyleSheet(QString("QLabel{background-color:rgb(%0,%1,%2);}").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void ChannelStripWidget::OnInstrumentClicked()
{
    if (this->_channel->ChannelInput() == 0)
    {
        this->PickInstrument(this->ui->btnEdit->mapToGlobal(QPoint()));
    }
    else
    {
        // TODO : edit instrument here
    }
}

void ChannelStripWidget::InstrumentPicked(QAction* action)
{
    if (QString(action->data().typeName()) == QString("int"))
    {
        MixerBus* bus = Mixer::Instance().GetBus(action->data().toInt());
        this->_channel->SetChannelInput(bus);
    }
    else
    {
        MixerInstrument* instrument = (MixerInstrument*)action->data().value<void*>();
        if (instrument == 0)
            instrument = Mixer::Instance().AddInstrument("Default instrument");

        this->_channel->SetChannelInput(instrument);
    }
}

void ChannelStripWidget::ChangeChannelInput(MixerChannelInput* instrument)
{
    if (instrument != 0)
    {
        this->ui->btnEdit->setText(instrument->GetName());
    }
}

void ChannelStripWidget::PickInstrument(const QPoint &pos)
{
    QMenu m(this);
    connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(InstrumentPicked(QAction*)));

    // Option to add channel
    QAction* newaction = new QAction("New instrument", this);
    newaction->setData(QVariant::fromValue((void*)0));
    m.addAction(newaction);

    m.addSeparator();

    for (QList<MixerInstrument*>::iterator itr = Mixer::Instance().Instruments().begin(); itr != Mixer::Instance().Instruments().end(); ++itr)
    {
        MixerInstrument* instrument = (MixerInstrument*)*itr;
        QAction* tmp = new QAction(instrument->GetInstrument()->Pname.c_str(), this);
        tmp->setData(QVariant::fromValue((void*)instrument));
        m.addAction(tmp);
    }

    m.addSeparator();

    QMenu* busses = new QMenu("Busses");
    m.addMenu(busses);
    for (int i = 0; i < MAX_BUS_COUNT; i++)
    {
        QAction* tmp = new QAction(QString("Bus ") + QString::number(i), this);
        tmp->setData(QVariant::fromValue(i));
        busses->addAction(tmp);
    }

    m.exec(pos);

    while (m.actions().empty() == false)
    {
        QAction* a = m.actions().back();
        m.actions().pop_back();
        delete a;
    }
}
