#include "channelstripcontainer.h"
#include "ui_channelstripcontainer.h"
#include "masterchannelstripwidget.h"
#include "mainwindow.h"
#include "sequencer/sequencer.h"
#include "mixer/mixer.h"
#include <QScrollBar>

ChannelStripContainer::ChannelStripContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelStripContainer)
{
    ui->setupUi(this);

    QVBoxLayout* layout = (QVBoxLayout*)this->ui->channelsContent->layout();
    layout->addWidget(new MasterChannelStripWidget(this));

    connect(this->ui->btnAddChannel, SIGNAL(clicked()), this, SLOT(AddChannel()));
    connect(&Mixer::Instance(), SIGNAL(ChannelAdded(MixerChannel*)), this, SLOT(OnChannelAdded(MixerChannel*)));
    connect(&Mixer::Instance(), SIGNAL(ChannelRemoved(MixerChannel*)), this, SLOT(OnChannelRemoved(MixerChannel*)));

    this->ui->stripLayout->setAlignment(Qt::AlignLeft);
    connect(this->ui->channelsScrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    this->_lastMax = this->ui->channelsScrollArea->verticalScrollBar()->maximum();
}

ChannelStripContainer::~ChannelStripContainer()
{
    delete ui;
}

void ChannelStripContainer::OnChannelAdded(MixerChannel* channel)
{
    QVBoxLayout* layout = (QVBoxLayout*)this->ui->channelsContent->layout();

    ChannelStripWidget* widget = new ChannelStripWidget();
    layout->insertWidget(layout->count() - 1, widget, 0, Qt::AlignLeft);
    widget->SetChannel(channel);
}

void ChannelStripContainer::OnChannelRemoved(MixerChannel* channel)
{
    ChannelStripWidget* widget = this->GetWidgetByChannel(channel);
    if (widget != 0)
    {
        ((QVBoxLayout*)this->ui->channelsContent->layout())->removeWidget(widget);
        delete widget;
        delete channel;
    }
}

ChannelStripWidget* ChannelStripContainer::GetWidgetByChannel(MixerChannel* channel)
{
    QVBoxLayout* layout = (QVBoxLayout*)this->ui->channelsContent->layout();

    for (int i = 0; i < layout->count(); i++)
    {
        ChannelStripWidget* widget = dynamic_cast<ChannelStripWidget*>(layout->itemAt(i)->widget());
        if (widget != 0 && widget->GetChannel() == channel)
            return widget;
    }

    return 0;
}

void ChannelStripContainer::AddChannel()
{
    Mixer::Instance().AddChannel("Default");
}

void ChannelStripContainer::moveScrollBarToBottom(int min, int max)
{
    Q_UNUSED(min);
    int currentScrollValue = this->ui->channelsScrollArea->verticalScrollBar()->value();
    this->ui->channelsScrollArea->verticalScrollBar()->setValue(currentScrollValue - (this->_lastMax - max));
    this->_lastMax = max;
}
