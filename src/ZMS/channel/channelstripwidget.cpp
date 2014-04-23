#include "channelstripwidget.h"
#include "ui_channelstripwidget.h"
#include "sequencer/sequencer.h"
#include "mixer/mixer.h"
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
    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnEdit, SIGNAL(clicked()), this, SLOT(OnEditClicked()));
}

ChannelStripWidget::~ChannelStripWidget()
{
    delete ui;
}

void ChannelStripWidget::SetChannelColor(const QColor& color)
{
    this->ui->lblName->setStyleSheet(QString("QLabel{background-color:rgb(%0,%1,%2);}").arg(color.red()).arg(color.green()).arg(color.blue()));
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
    return false;
}

void ChannelStripWidget::SetChannel(MixerChannel *channel)
{
    this->_channel = channel;
    if (this->_channel != 0)
    {
        this->ui->volume->setValue(this->_channel->GetVolume());
        this->ui->lblName->setText(this->_channel->GetName());
        this->SetChannelColor(this->_channel->GetColor());
        connect(this->_channel, SIGNAL(NameChanged(QString)), this->ui->lblName, SLOT(setText(QString)));
    }
}

void ChannelStripWidget::OnChannelUpdated(MixerChannel* channel)
{
    if (channel == this->_channel)
    {
        this->ui->volume->setValue(this->_channel->GetVolume());
        this->ui->lblName->setText(this->_channel->GetName());
        this->SetChannelColor(this->_channel->GetColor());
    }
}

void ChannelStripWidget::OnVolumeSliderChanged(int value)
{
    this->_channel->SetVolume(value);
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
        Sequencer::Inst().CurrentSongIsUpdated();
    }
}

void ChannelStripWidget::OnEditClicked()
{
    emit ActivateChannel(this->_channel);
}
