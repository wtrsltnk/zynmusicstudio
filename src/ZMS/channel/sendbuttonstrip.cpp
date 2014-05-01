#include "sendbuttonstrip.h"
#include "ui_sendbuttonstrip.h"
#include "mixer/mixersendsink.h"

SendButton::SendButton(MixerSendSink *sink)
    : _sink(sink)
{ }

SendButton::~SendButton()
{ }

SendButtonStrip::SendButtonStrip(MixerSendSource* sends, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendButtonStrip),
    _sends(sends)
{
    ui->setupUi(this);

    if (this->_sends != 0)
    {
        connect(this->_sends, SIGNAL(SinkAdded(MixerSendSink*)), this, SLOT(AddSink(MixerSendSink*)));
        connect(this->_sends, SIGNAL(SinkRemoved(MixerSendSink*)), this, SLOT(RemoveSink(MixerSendSink*)));
    }
}

SendButtonStrip::~SendButtonStrip()
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        SendButton* btn = dynamic_cast<SendButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            this->ui->buttonlayout->removeWidget(btn);
            delete btn;
        }
    }

    delete ui;
}

void SendButtonStrip::AddSink(MixerSendSink* sink)
{
    SendButton* btn = new SendButton(sink);
    this->ui->buttonlayout->insertWidget(this->ui->buttonlayout->count() - 2, btn);
}

void SendButtonStrip::RemoveSink(MixerSendSink* sink)
{
    for (int i = 0; i < this->ui->buttonlayout->count(); i++)
    {
        SendButton* btn = dynamic_cast<SendButton*>(this->ui->buttonlayout->itemAt(i)->widget());
        if (btn != 0)
        {
            if (btn->Sink() == sink)
            {
                this->ui->buttonlayout->removeWidget(btn);
                delete btn;
            }
        }
    }
}
