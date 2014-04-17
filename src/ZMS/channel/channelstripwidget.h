#ifndef CHANNELSTRIPWIDGET_H
#define CHANNELSTRIPWIDGET_H

#include <QFrame>
#include "../Misc/Channel.h"

namespace Ui {
class ChannelStripWidget;
}

class ChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChannelStripWidget(QWidget *parent = 0);
    virtual ~ChannelStripWidget();

    Channel* GetChannel() { return this->_channel; }
    void SetChannel(Channel* channel) { this->_channel = channel; this->OnChannelChanged(channel); }

signals:
    void ActivateChannel(Channel* channel);

public slots:
    void OnChannelChanged(Channel* channel);
    void OnVolumeSliderChanged(int value);
    void OnPanDialChanged(int value);
    void OnCloseClicked();
    void OnEditClicked();
    void OnMinimumHeightEffectStripChanged(int height);

private:
    Ui::ChannelStripWidget *ui;
    Channel* _channel;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // CHANNELSTRIPWIDGET_H
