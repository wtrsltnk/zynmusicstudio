#include "trackproperties.h"
#include "ui_trackproperties.h"
#include "sequencer/sequencer.h"
#include "mixer/mixer.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QResizeEvent>
#include <QMenu>
#include <QList>
#include <QVariant>
#include <iostream>

using namespace std;

TrackProperties::TrackProperties(SequencerTrack* track) :
    QFrame(),
    ui(new Ui::TrackProperties),
    _track(track)
{
    ui->setupUi(this);

    this->ui->comboBox->installEventFilter(this);
    this->ui->btnChannel->installEventFilter(this);

    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnChannel, SIGNAL(clicked()), this, SLOT(OnChangeChannel()));

    this->ui->label->setText(this->_track->Title());
    this->ui->comboBox->addItem("Volume");
    this->ui->comboBox->addItem("Pan");
    this->ui->comboBox->addItem("Calf EQ - LB");

    this->ui->label->installEventFilter(this);

    if (this->_track != 0)
    {
        connect(this->_track, SIGNAL(ChannelChanged(MixerChannel*)), this, SLOT(ChannelChanged(MixerChannel*)));
        if (this->_track->GetChannel() != 0)
            connect(this->_track->GetChannel(), SIGNAL(NameChanged(QString)), this, SLOT(TrackChannelNameChanged(QString)));
    }
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
            this->_track->SetTitle(dlg.textValue());

        return true;
    }

    if (o == this->ui->btnChannel && e->type() == QEvent::ContextMenu)
    {
        QContextMenuEvent* contextEvent = (QContextMenuEvent*)e;
        this->PickChannel(contextEvent->globalPos());
        return true;
    }

    if (e->type() == QEvent::MouseButtonPress)
    {
        Sequencer::Inst().CurrentSong()->SetCurrentTrack(this->_track);
        return true;
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

void TrackProperties::OnChangeChannel()
{
    if (this->_track->GetChannel() == 0)
    {
        this->PickChannel(this->ui->btnChannel->mapToGlobal(this->ui->btnChannel->pos()));
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

void TrackProperties::OnChannelUpdated(MixerChannel* channel)
{
    if (channel == this->_track->GetChannel())
    {
        this->ui->btnChannel->setText(this->_track->GetChannel()->GetName());
    }
}

void TrackProperties::ChannelPicked(QAction* action)
{
    MixerChannel* channel = action->data().value<MixerChannel*>();
    if (channel == 0)
        channel = Mixer::Instance().AddChannel("Default");

    if (this->_track->GetChannel() != 0)
        disconnect(this->_track->GetChannel(), SIGNAL(NameChanged(QString)), this, SLOT(TrackChannelNameChanged(QString)));
    this->_track->SetChannel(channel);

    if (this->_track->GetChannel() != 0)
        this->ui->btnChannel->setText(this->_track->GetChannel()->GetName());
}

void TrackProperties::ChannelChanged(MixerChannel* channel)
{
    if (channel != 0)
    {
        this->ui->btnChannel->setText(channel->GetName());
        connect(channel, SIGNAL(NameChanged(QString)), this, SLOT(TrackChannelNameChanged(QString)));
    }
}

void TrackProperties::TrackChannelNameChanged(const QString& title)
{
    this->ui->btnChannel->setText(title);
}

void TrackProperties::PickChannel(const QPoint &pos)
{
    QMenu m(this);
    connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(ChannelPicked(QAction*)));

    for (QList<MixerChannel*>::iterator itr = Mixer::Instance().Channels().begin(); itr != Mixer::Instance().Channels().end(); ++itr)
    {
        MixerChannel* channel = (MixerChannel*)*itr;
        QAction* tmp = new QAction(channel->GetName(), this);
        tmp->setData(qVariantFromValue(channel));
        m.addAction(tmp);
    }
    m.addSeparator();

    // Option to add channel
    QAction* newaction = new QAction("New channel", this);
    newaction->setData(QVariant::fromValue((void*)0));
    m.addAction(newaction);

    m.exec(pos);

    while (m.actions().empty() == false)
    {
        QAction* a = m.actions().back();
        m.actions().pop_back();
        delete a;
    }
}
