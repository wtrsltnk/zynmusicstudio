#include "masterchannelstripwidget.h"
#include "ui_masterchannelstripwidget.h"
#include "../Misc/Master.h"
#include "../Effects/EffectMgr.h"
#include "../Nio/InMgr.h"
#include "../Nio/OutMgr.h"
#include "../Nio/EngineMgr.h"
#include <QTimer>
#include <QPainter>
#include <QMenu>

MasterChannelStripWidget::MasterChannelStripWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MasterChannelStripWidget)
{
    ui->setupUi(this);

    this->ui->sysmaster->setValue(Master::getInstance().Pvolume);

    this->ui->btnMidi->setText(Master::getInstance().engineManager->Input()->getSource().c_str());
    connect(this->ui->btnMidi, SIGNAL(clicked()), this, SLOT(OnShowMidiDevices()));
    this->ui->btnAudio->setText(Master::getInstance().engineManager->Output()->getSink().c_str());
    connect(this->ui->btnAudio, SIGNAL(clicked()), this, SLOT(OnShowAudioDevices()));

    this->ui->sysvuL->installEventFilter(this);
    this->ui->sysvuR->installEventFilter(this);

    this->_vutimer = new QTimer();
    this->_vutimer->setInterval(1000/40);
    this->_vutimer->start();
    connect(this->_vutimer, SIGNAL(timeout()), this, SLOT(OnVuTimer()));

    connect(this->ui->sysmaster, SIGNAL(valueChanged(int)), this, SLOT(OnMasterGainChanged(int)));
    connect(this->ui->effectStrip, SIGNAL(OnMinimumHeightChanged(int)), this, SLOT(OnMinimumHeightEffectStripChanged(int)));
}

MasterChannelStripWidget::~MasterChannelStripWidget()
{
    delete this->_vutimer;
    delete ui;
}

bool MasterChannelStripWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->sysvuL && event->type() == QEvent::Paint)
    {
        int lx = ui->sysvuL->width() * 0.6; int ly = ui->sysvuL->height();

        int idbl=(int) (this->dbl*ly);
        int irmsdbl=(int) (this->rmsdbl*ly);

        QPainter painter;
        painter.begin(ui->sysvuL);

        painter.fillRect(ui->sysvuL->width() * 0.2,ly-idbl,lx,idbl, QBrush(QColor::fromRgb(0, 191, 255), Qt::Dense7Pattern));
        painter.fillRect(ui->sysvuL->width() * 0.2,0,lx,ly-idbl, QBrush(Qt::white, Qt::Dense7Pattern));
        painter.fillRect(ui->sysvuL->width() * 0.2,ly-irmsdbl-1,lx,3, QBrush(QColor::fromRgb(0, 143, 191), Qt::SolidPattern));

        painter.end();
        return true;
    }
    if (watched == ui->sysvuR && event->type() == QEvent::Paint)
    {
        int lx=ui->sysvuR->width() * 0.6; int ly=ui->sysvuR->height();

        int idbr=(int) (this->dbr*ly);
        int irmsdbr=(int) (this->rmsdbr*ly);

        QPainter painter;
        painter.begin(ui->sysvuR);

        painter.fillRect(ui->sysvuR->width() * 0.2,ly-idbr,lx,idbr, QBrush(QColor::fromRgb(0, 191, 255), Qt::Dense7Pattern));
        painter.fillRect(ui->sysvuR->width() * 0.2,0,lx,ly-idbr, QBrush(Qt::white, Qt::Dense7Pattern));
        painter.fillRect(ui->sysvuR->width() * 0.2,ly-irmsdbr-1,lx,3, QBrush(QColor::fromRgb(0, 143, 191), Qt::SolidPattern));

        painter.end();
        return true;
    }
    return false;
}

void MasterChannelStripWidget::OnMasterGainChanged(int value)
{
    Master::getInstance().setPvolume(value);
}

void MasterChannelStripWidget::OnVuTimer()
{
    // This part is copied from MasterUI.fl by Nasca Octavian Paul
    static float olddbl = 0;
    static float olddbr = 0;
    static float oldrmsdbl = 0;
    static float oldrmsdbr = 0;
#define MIN_DB (-48)

    vuData data = Master::getInstance().getVuData();

    this->dbl=rap2dB(data.outpeakl);
    this->dbr=rap2dB(data.outpeakr);
    this->dbl=(MIN_DB-this->dbl)/MIN_DB;
    if (this->dbl<0.0) this->dbl=0.0;
      else if (this->dbl>1.0)this->dbl=1.0;

    this->dbr=(MIN_DB-this->dbr)/MIN_DB;
    if (this->dbr<0.0) this->dbr=0.0;
      else if (this->dbr>1.0) this->dbr=1.0;

    this->dbl=this->dbl*0.4+olddbl*0.6;
    this->dbr=this->dbr*0.4+olddbr*0.6;

    olddbl=this->dbl;
    olddbr=this->dbr;

    //compute RMS - start
    this->rmsdbl=rap2dB(data.rmspeakl);
    this->rmsdbr=rap2dB(data.rmspeakr);
    this->rmsdbl=(MIN_DB-this->rmsdbl)/MIN_DB;
    if (this->rmsdbl<0.0) this->rmsdbl=0.0;
      else if (this->rmsdbl>1.0) this->rmsdbl=1.0;

    this->rmsdbr=(MIN_DB-this->rmsdbr)/MIN_DB;
    if (this->rmsdbr<0.0) this->rmsdbr=0.0;
      else if (this->rmsdbr>1.0) this->rmsdbr=1.0;

    this->rmsdbl=this->rmsdbl*0.4+oldrmsdbl*0.6;
    this->rmsdbr=this->rmsdbr*0.4+oldrmsdbr*0.6;

    oldrmsdbl=rmsdbl;
    oldrmsdbr=rmsdbr;

    this->ui->sysvuL->repaint();
    this->ui->sysvuR->repaint();
}

void MasterChannelStripWidget::OnShowMidiDevices()
{
    QMenu m;
    std::list<Engine *>& en = Master::getInstance().engineManager->Engines();
    for (std::list<Engine *>::iterator i = en.begin(); i != en.end(); ++i)
    {
        Engine* e = dynamic_cast<Engine*>(*i);
        if (e != 0 && e->IsMidiIn())
        {
            QAction* a = m.addAction(e->Name().c_str());
            connect(a, SIGNAL(triggered()), this, SLOT(OnSelectMidiDevice()));
        }
    }

    m.exec(QCursor::pos());
}

void MasterChannelStripWidget::OnSelectMidiDevice()
{
    QString sel = ((QAction*)sender())->text();
    if (Master::getInstance().engineManager->Input()->setSource(sel.toStdString()))
        this->ui->btnMidi->setText(sel);
}

void MasterChannelStripWidget::OnShowAudioDevices()
{
    QMenu m;
    std::list<Engine *>& en = Master::getInstance().engineManager->Engines();
    for (std::list<Engine *>::iterator i = en.begin(); i != en.end(); ++i)
    {
        Engine* e = dynamic_cast<Engine*>(*i);
        if (e != 0 && e->IsAudioOut())
        {
            QAction* a = m.addAction(e->Name().c_str());
            connect(a, SIGNAL(triggered()), this, SLOT(OnSelectAudioDevice()));
        }
    }

    m.exec(QCursor::pos());
}

void MasterChannelStripWidget::OnSelectAudioDevice()
{
    QString sel = ((QAction*)sender())->text();
    if (Master::getInstance().engineManager->Output()->setSink(sel.toStdString()))
        this->ui->btnAudio->setText(sel);
}

void MasterChannelStripWidget::OnMinimumHeightEffectStripChanged(int height)
{
    this->setMinimumHeight(350 + height);
}
