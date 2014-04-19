#include "trackproperties.h"
#include "ui_trackproperties.h"
#include "trackareaselectchanneldialog.h"
#include "sequencer/sequencer.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QResizeEvent>
#include <iostream>

using namespace std;

TrackProperties::TrackProperties(SequencerTrack* track) :
    QFrame(),
    ui(new Ui::TrackProperties),
    _track(track)
{
    ui->setupUi(this);

    this->ui->comboBox->installEventFilter(this);

    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnChannel, SIGNAL(clicked()), this, SLOT(OnChangeChannel()));

    this->ui->label->setText(this->_track->Title());
    this->ui->comboBox->addItem("Volume");
    this->ui->comboBox->addItem("Pan");
    this->ui->comboBox->addItem("Calf EQ - LB");

    this->ui->label->installEventFilter(this);

    connect(&Sequencer::Inst(), SIGNAL(ChannelIsUpdated(Instrument*)), this, SLOT(OnChannelUpdated(Instrument*)));
}

TrackProperties::~TrackProperties()
{
    delete ui;
}

bool TrackProperties::eventFilter(QObject *o, QEvent *e)
{
    if (o == this->ui->label && e->type() == QEvent::MouseButtonDblClick)
    {
        QInputDialog dlg;
        dlg.setLabelText("Enter a new title:");
        dlg.setModal(true);
        dlg.setTextValue(this->ui->label->text());
        if (dlg.exec() == QDialog::Accepted)
        {
            this->_track->SetTitle(dlg.textValue());
            Sequencer::Inst().CurrentSongIsUpdated();
        }
        return true;
    }

    if (e->type() == QEvent::MouseButtonPress)
    {
        Sequencer::Inst().CurrentSong()->SetCurrentTrack(this->_track);
    }

    return false;
}

void TrackProperties::mousePressEvent(QMouseEvent* e)
{
    Sequencer::Inst().CurrentSong()->SetCurrentTrack(this->_track);
}

void TrackProperties::resizeEvent(QResizeEvent* e)
{
    this->ui->comboBox->setVisible(e->size().height() > 80);
}

void TrackProperties::OnCloseClicked()
{
    QMessageBox dlg;
    dlg.setModal(true);
    dlg.setText("Are you sure to remove this track?");
    dlg.setIcon(QMessageBox::Information);
    dlg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (dlg.exec() == QMessageBox::Yes)
    {
        Sequencer::Inst().CurrentSong()->RemoveTrack(this->_track);
        Sequencer::Inst().CurrentSongIsUpdated();
    }
}

void TrackProperties::UpdateWidget()
{
    this->ui->label->setText(this->_track->Title());

    if (this->_track == Sequencer::Inst().CurrentSong()->CurrentTrack())
        this->setStyleSheet("TrackProperties{background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #696969, stop:0.5 #666666, stop:1 #5F5F5F);}");
    else
        this->setStyleSheet("");
}

void TrackProperties::OnChangeChannel()
{
    TrackAreaSelectChannelDialog dlg;

    if (dlg.exec() == QDialog::Accepted && dlg.SelectedChannel() != 0)
    {
        this->_track->SetChannel(dlg.SelectedChannel());
        Sequencer::Inst().ChannelIsUpdated(dlg.SelectedChannel());
        this->ActivateChannel(this->_track->GetChannel());
    }
}
void TrackProperties::OnChannelUpdated(Instrument* channel)
{
    if (channel == this->_track->GetChannel())
    {
        this->ui->btnChannel->setText(this->_track->GetChannel()->Pname.c_str());
    }
}
