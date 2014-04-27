#include "channelstripwidget.h"
#include "ui_channelstripwidget.h"
#include "sequencer/sequencer.h"
#include "mixer/mixer.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QMenu>
#include <iostream>

using namespace std;

ChannelStripWidget::ChannelStripWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ChannelStripWidget),
    _channel(0)
{
    ui->setupUi(this);

    this->ui->lblName->installEventFilter(this);
    this->ui->btnEdit->installEventFilter(this);
    this->ui->btnOutput->installEventFilter(this);

    connect(this->ui->btnClose, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    connect(this->ui->btnEdit, SIGNAL(clicked()), this, SLOT(OnInstrumentClicked()));
}

ChannelStripWidget::~ChannelStripWidget()
{
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
        Sequencer::Inst().CurrentSongIsUpdated();
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

    if (o == this->ui->btnOutput && e->type() == QEvent::ContextMenu)
    {
        QContextMenuEvent* contextEvent = (QContextMenuEvent*)e;
        this->PickOutput(contextEvent->globalPos());
        return true;
    }

    return false;
}

void ChannelStripWidget::SetChannelColor(const QColor& color)
{
    this->ui->lblName->setStyleSheet(QString("QLabel{background-color:rgb(%0,%1,%2);}").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void ChannelStripWidget::SetChannel(MixerChannel *channel)
{
    if (this->_channel != 0)
    {
        disconnect(this->_channel, SIGNAL(VolumeChanged(int)), this->ui->volume, SLOT(setValue(int)));
        disconnect(this->ui->volume, SIGNAL(valueChanged(int)), this->_channel, SLOT(SetVolume(int)));
        disconnect(this->_channel, SIGNAL(NameChanged(QString)), this->ui->lblName, SLOT(setText(QString)));
        disconnect(this->_channel, SIGNAL(ColorChanged(QColor)), this, SLOT(SetChannelColor(QColor)));
        disconnect(this->_channel, SIGNAL(InstrumentChanged(Instrument*)), this, SLOT(ChangeChannelInstrument(Instrument*)));
        disconnect(this->_channel, SIGNAL(SinkChanged(MixerSink*)), this, SLOT(ChangeChannelOutput(MixerSink*)));
    }

    this->_channel = channel;

    if (this->_channel != 0)
    {
        connect(this->_channel, SIGNAL(VolumeChanged(int)), this->ui->volume, SLOT(setValue(int)));
        connect(this->ui->volume, SIGNAL(valueChanged(int)), this->_channel, SLOT(SetVolume(int)));
        connect(this->_channel, SIGNAL(NameChanged(QString)), this->ui->lblName, SLOT(setText(QString)));
        connect(this->_channel, SIGNAL(ColorChanged(QColor)), this, SLOT(SetChannelColor(QColor)));
        connect(this->_channel, SIGNAL(InstrumentChanged(Instrument*)), this, SLOT(ChangeChannelInstrument(Instrument*)));
        connect(this->_channel, SIGNAL(SinkChanged(MixerSink*)), this, SLOT(ChangeChannelOutput(MixerSink*)));

        this->ui->volume->setValue(this->_channel->GetVolume());
        this->ui->lblName->setText(this->_channel->GetName());
        this->SetChannelColor(this->_channel->GetColor());
        this->ChangeChannelInstrument(this->_channel->GetInstrument());
        this->ChangeChannelOutput(this->_channel->Sink());
    }
}

void ChannelStripWidget::OnInstrumentClicked()
{
    if (this->_channel->GetInstrument() == 0)
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
    Instrument* instrument = (Instrument*)action->data().value<void*>();
    if (instrument == 0)
        instrument = Mixer::Instance().AddInstrument("Default instrument");

    this->_channel->SetInstrument(instrument);
}

void ChannelStripWidget::ChangeChannelInstrument(Instrument* instrument)
{
    if (instrument != 0)
    {
        this->ui->btnEdit->setText(instrument->Pname.c_str());
    }
}

void ChannelStripWidget::PickInstrument(const QPoint &pos)
{
    QMenu m(this);
    connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(InstrumentPicked(QAction*)));

    for (QList<Instrument*>::iterator itr = Mixer::Instance().Instruments().begin(); itr != Mixer::Instance().Instruments().end(); ++itr)
    {
        Instrument* instrument = (Instrument*)*itr;
        QAction* tmp = new QAction(instrument->Pname.c_str(), this);
        tmp->setData(QVariant::fromValue((void*)instrument));
        m.addAction(tmp);
    }
    m.addSeparator();

    // Option to add channel
    QAction* newaction = new QAction("New instrument", this);
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

void ChannelStripWidget::OutputPicked(QAction* action)
{
    MixerSink* sink = (MixerSink*)action->data().value<void*>();
    if (sink != 0)
        this->_channel->SetSink(sink);
}

void ChannelStripWidget::ChangeChannelOutput(MixerSink* sink)
{
    if (sink != 0)
    {
        this->ui->btnOutput->setText(sink->Title());
    }
}

void ChannelStripWidget::PickOutput(const QPoint &pos)
{
    QMenu m(this);
    connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(OutputPicked(QAction*)));

    for (QList<MixerSink*>::iterator itr = Mixer::Instance().Outputs().begin(); itr != Mixer::Instance().Outputs().end(); ++itr)
    {
        MixerSink* sink = (MixerSink*)*itr;
        QAction* tmp = new QAction(sink->Title(), this);
        tmp->setData(QVariant::fromValue((void*)sink));
        m.addAction(tmp);
    }

    m.exec(pos);

    while (m.actions().empty() == false)
    {
        QAction* a = m.actions().back();
        m.actions().pop_back();
        delete a;
    }
}
