#include "channelstripcontainer.h"
#include "ui_channelstripcontainer.h"
#include "masterchannelstripwidget.h"
#include "mainwindow.h"
#include "sequencer/sequencer.h"
#include "../Misc/Master.h"
#include <QScrollBar>

ChannelStripContainer::ChannelStripContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelStripContainer)
{
    ui->setupUi(this);

    QVBoxLayout* layout = (QVBoxLayout*)this->ui->channelsContent->layout();
    layout->addWidget(new MasterChannelStripWidget(this));

    connect(&Sequencer::Inst(), SIGNAL(CurrentSongIsUpdated()), this, SLOT(UpdateChannels()));
    connect(this->ui->btnAddChannel, SIGNAL(clicked()), this, SLOT(AddChannel()));

    this->ui->stripLayout->setAlignment(Qt::AlignLeft);
    connect(this->ui->channelsScrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    this->_lastMax = this->ui->channelsScrollArea->verticalScrollBar()->maximum();
}

ChannelStripContainer::~ChannelStripContainer()
{
    delete ui;
}

void ChannelStripContainer::UpdateChannels()
{
    QVBoxLayout* layout = (QVBoxLayout*)this->ui->channelsContent->layout();

    for (int i = 0; i < layout->count() - 1; i++)
    {
        ChannelStripWidget* widget = dynamic_cast<ChannelStripWidget*>(layout->itemAt(i)->widget());
        if (widget != 0)
        {
            if (Master::getInstance().channelIndex(widget->GetChannel()) == -1)
            {
                layout->removeWidget(widget);
                delete widget;
                i = 0;
            }
        }
    }

    for (std::vector<Channel*>::iterator i = Master::getInstance().Channels().begin();
         i != Master::getInstance().Channels().end(); ++i)
    {
        ChannelStripWidget* widget = this->GetWidgetByChannel(*i);
        if (widget == 0)
        {
            widget = new ChannelStripWidget();
            connect(&Sequencer::Inst(), SIGNAL(ChannelIsUpdated(Channel*)), widget, SLOT(OnChannelChanged(Channel*)));
            connect(widget, SIGNAL(ActivateChannel(Channel*)), this->_mainParent, SLOT(OnChannelIsActivated(Channel*)));
            layout->insertWidget(layout->count() - 1, widget, 0, Qt::AlignLeft);
        }
        widget->SetChannel(*i);
    }
}

ChannelStripWidget* ChannelStripContainer::GetWidgetByChannel(Channel* channel)
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
    Master::getInstance().addChannel();
    Sequencer::Inst().CurrentSongIsUpdated();
}

void ChannelStripContainer::moveScrollBarToBottom(int min, int max)
{
    Q_UNUSED(min);
    int currentScrollValue = this->ui->channelsScrollArea->verticalScrollBar()->value();
    this->ui->channelsScrollArea->verticalScrollBar()->setValue(currentScrollValue - (this->_lastMax - max));
    this->_lastMax = max;
}
