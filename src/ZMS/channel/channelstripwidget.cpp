#include "channelstripwidget.h"
#include "ui_channelstripwidget.h"
#include <sequencer/sequencer.h>
#include <../Misc/Master.h>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>

using namespace std;

ChannelStripWidget::ChannelStripWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ChannelStripWidget)
{
    ui->setupUi(this);

    this->ui->lblName->installEventFilter(this);

    connect(this->ui->volume, SIGNAL(valueChanged(int)), this, SLOT(OnVolumeSliderChanged(int)));
    connect(this->ui->pan, SIGNAL(valueChanged(int)), this, SLOT(OnPanDialChanged(int)));
    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnEdit, SIGNAL(clicked()), this, SLOT(OnEditClicked()));
}

ChannelStripWidget::~ChannelStripWidget()
{
    delete ui;
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
            this->_channel->Pname = dlg.textValue().toStdString();
            Sequencer::Inst().ChannelIsUpdated(this->_channel);
        }
        return true;
    }
    return false;
}

void ChannelStripWidget::OnChannelChanged(Instrument* channel)
{
    if (channel == this->_channel)
    {
        this->ui->volume->setValue(this->_channel->Pvolume);
        this->ui->pan->setValue(this->_channel->Ppanning);
        this->ui->lblName->setText(this->_channel->Pname.c_str());
    }
}

void ChannelStripWidget::OnVolumeSliderChanged(int value)
{
    this->_channel->setPvolume(value);
}

void ChannelStripWidget::OnPanDialChanged(int value)
{
    this->_channel->setPpanning(value);
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
        Master::getInstance().removeInstrument(this->_channel);
        Sequencer::Inst().CurrentSongIsUpdated();
    }
}

void ChannelStripWidget::OnEditClicked()
{
    this->ActivateChannel(this->_channel);
}
