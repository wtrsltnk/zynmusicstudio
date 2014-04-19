#ifndef CHANNELSTRIPWIDGET_H
#define CHANNELSTRIPWIDGET_H

#include <QFrame>
#include "../Misc/Instrument.h"

namespace Ui {
class ChannelStripWidget;
}

class ChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChannelStripWidget(QWidget *parent = 0);
    virtual ~ChannelStripWidget();

    Instrument* GetChannel() { return this->_channel; }
    void SetChannel(Instrument* channel) { this->_channel = channel; this->OnChannelChanged(channel); }

signals:
    void ActivateChannel(Instrument* channel);

public slots:
    void OnChannelChanged(Instrument* channel);
    void OnVolumeSliderChanged(int value);
    void OnPanDialChanged(int value);
    void OnCloseClicked();
    void OnEditClicked();
    void OnMinimumHeightEffectStripChanged(int height);

private:
    Ui::ChannelStripWidget *ui;
    Instrument* _channel;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // CHANNELSTRIPWIDGET_H
