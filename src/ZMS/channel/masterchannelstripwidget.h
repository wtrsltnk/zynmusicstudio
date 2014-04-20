#ifndef MASTERCHANNELSTRIPWIDGET_H
#define MASTERCHANNELSTRIPWIDGET_H

#include <QFrame>

namespace Ui {
class MasterChannelStripWidget;
}

class MasterChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MasterChannelStripWidget(QWidget *parent = 0);
    ~MasterChannelStripWidget();

protected slots:
    void OnVuTimer();
    void OnMasterGainChanged(int value);
    void OnShowMidiDevices();
    void OnSelectMidiDevice();
    void OnShowAudioDevices();
    void OnSelectAudioDevice();

private:
    Ui::MasterChannelStripWidget *ui;
    QTimer* _vutimer;

    float dbl;
    float dbr;
    float rmsdbl;
    float rmsdbr;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // MASTERCHANNELSTRIPWIDGET_H
