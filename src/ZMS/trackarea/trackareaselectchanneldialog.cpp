#include "trackareaselectchanneldialog.h"
#include "ui_trackareaselectchanneldialog.h"
#include "sequencer/sequencer.h"
#include "../Misc/Master.h"

TrackAreaSelectChannelDialog::TrackAreaSelectChannelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrackAreaSelectChannelDialog),
    _selectedChannel(0)
{
    ui->setupUi(this);

    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(OnAccepted()));
    connect(this->ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnAddChannel()));
    connect(this->ui->btnRemove, SIGNAL(clicked()), this, SLOT(OnRemoveChannel()));

    this->ShowChannels();
}

TrackAreaSelectChannelDialog::~TrackAreaSelectChannelDialog()
{
    delete ui;
}

void TrackAreaSelectChannelDialog::OnAccepted()
{
    if (this->ui->lstParts->selectedItems().count() > 0)
    {
        QListWidgetItem* item = this->ui->lstParts->selectedItems().at(0);
        if (item != 0)
        {
            Instrument* channel = (Instrument*)item->data(23).value<void*>();
            if (channel != 0)
                this->_selectedChannel = channel;
        }
    }
}

void TrackAreaSelectChannelDialog::OnAddChannel()
{
    Master::getInstance().addChannel();
    Sequencer::Inst().CurrentSongIsUpdated();
    this->ShowChannels();
}

void TrackAreaSelectChannelDialog::OnRemoveChannel()
{
    if (this->ui->lstParts->selectedItems().count() > 0)
    {
        QListWidgetItem* item = this->ui->lstParts->selectedItems().at(0);
        if (item != 0)
        {
            Instrument* channel = (Instrument*)item->data(23).value<void*>();
            if (channel != 0)
            {
                Master::getInstance().removeChannel(channel);
                this->ShowChannels();
            }
        }
    }
}

void TrackAreaSelectChannelDialog::ShowChannels()
{
    this->ui->lstParts->clear();
    for (std::vector<Instrument *>::iterator i = Master::getInstance().Channels().begin();
         i != Master::getInstance().Channels().end(); ++i)
    {
        Instrument* part = *i;
        QListWidgetItem* item = new QListWidgetItem(this->ui->lstParts);
        item->setText(part->Pname.c_str());
        item->setData(23, QVariant::fromValue((void*)part));
        if (part == this->_selectedChannel)
        {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            item->setSelected(true);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
        this->ui->lstParts->addItem(item);
    }
}
