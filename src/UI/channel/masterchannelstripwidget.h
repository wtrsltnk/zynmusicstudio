#ifndef MASTERCHANNELSTRIPWIDGET_H
#define MASTERCHANNELSTRIPWIDGET_H

#include <QFrame>
#include "effectbuttonstrip.h"

namespace Ui {
class MasterChannelStripWidget;
}

class MixerMaster;

class MasterChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MasterChannelStripWidget(QWidget *parent = 0);
    ~MasterChannelStripWidget();

protected slots:
    void OnVuTimer();
    void OnShowMidiDevices();
    void OnSelectMidiDevice();
    void OnShowAudioDevices();
    void OnSelectAudioDevice();

private:
    Ui::MasterChannelStripWidget *ui;
    EffectButtonStrip *uieffectButtons;
    MixerMaster* _master;
    QTimer* _vutimer;

    float dbl;
    float dbr;
    float rmsdbl;
    float rmsdbr;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // MASTERCHANNELSTRIPWIDGET_H
