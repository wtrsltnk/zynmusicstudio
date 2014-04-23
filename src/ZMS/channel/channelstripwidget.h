#ifndef CHANNELSTRIPWIDGET_H
#define CHANNELSTRIPWIDGET_H

#include <QFrame>
#include "mixer/mixerchannel.h"

namespace Ui {
class ChannelStripWidget;
}

class ChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChannelStripWidget(QWidget *parent = 0);
    virtual ~ChannelStripWidget();

    MixerChannel* GetChannel() { return this->_channel; }
    void SetChannel(MixerChannel* channel);

signals:
    void ActivateChannel(MixerChannel* channel);

public slots:
    void OnChannelUpdated(MixerChannel* channel);
    void OnVolumeSliderChanged(int value);
    void OnCloseClicked();
    void OnEditClicked();
    void SetChannelColor(const QColor& color);

private:
    Ui::ChannelStripWidget *ui;
    MixerChannel* _channel;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // CHANNELSTRIPWIDGET_H
