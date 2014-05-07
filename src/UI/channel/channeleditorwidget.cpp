#include "channeleditorwidget.h"
#include "ui_channeleditorwidget.h"
#include "sequencer/sequencer.h"
#include <QInputDialog>

ChannelEditorWidget::ChannelEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelEditorWidget)
{
    ui->setupUi(this);

    this->ui->label->installEventFilter(this);

    connect(&Sequencer::Inst(), SIGNAL(ChannelIsUpdated(Instrument*)), this, SLOT(OnChannelUpdated(Instrument*)));

    connect(this->ui->chkEnableAdd, SIGNAL(toggled(bool)), this, SLOT(OnAddSynthEnabledChanged(bool)));
    connect(this->ui->chkEnableSub, SIGNAL(toggled(bool)), this, SLOT(OnSubSynthEnabledChanged(bool)));
    connect(this->ui->chkEnablePad, SIGNAL(toggled(bool)), this, SLOT(OnPadSynthEnabledChanged(bool)));
}

ChannelEditorWidget::~ChannelEditorWidget()
{
    delete ui;
}

bool ChannelEditorWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == this->ui->label && e->type() == QEvent::MouseButtonDblClick)
    {
        QInputDialog dlg;
        dlg.setLabelText("Enter a new title:");
        dlg.setModal(true);
        dlg.setTextValue(this->ui->label->text());
        if (dlg.exec() == QDialog::Accepted)
        {
            this->_channel->Pname = dlg.textValue().toStdString();
            Sequencer::Inst().ChannelIsUpdated(this->_channel);
        }
        return true;
    }
    return false;
}

void ChannelEditorWidget::OnChannelUpdated(Instrument* channel)
{
    if (this->_channel == channel)
    {
        this->ui->label->setText(this->_channel->Pname.c_str());
        this->ui->chkEnableAdd->setChecked(this->_channel->synths.Padenabled);
        this->ui->chkEnableSub->setChecked(this->_channel->synths.Psubenabled);
        this->ui->chkEnablePad->setChecked(this->_channel->synths.Ppadenabled);
    }
}

void ChannelEditorWidget::OnAddSynthEnabledChanged(bool state)
{
    this->ui->synths->tabBar()->setTabEnabled(1, state);
    this->_channel->synths.Padenabled = (state ? 1 : 0);
}

void ChannelEditorWidget::OnSubSynthEnabledChanged(bool state)
{
    this->ui->synths->tabBar()->setTabEnabled(2, state);
    this->_channel->synths.Psubenabled = (state ? 1 : 0);
}

void ChannelEditorWidget::OnPadSynthEnabledChanged(bool state)
{
    this->ui->synths->tabBar()->setTabEnabled(3, state);
    this->_channel->synths.Ppadenabled = (state ? 1 : 0);
}

void ChannelEditorWidget::SetChannel(Instrument *channel)
{
    this->_channel = channel;
    if (this->_channel != 0)
        this->OnChannelUpdated(this->_channel);
}
