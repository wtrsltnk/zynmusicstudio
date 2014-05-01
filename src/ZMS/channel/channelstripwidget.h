#ifndef CHANNELSTRIPWIDGET_H
#define CHANNELSTRIPWIDGET_H

#include <QFrame>
#include "mixer/mixerchannel.h"
#include "mixer/mixerchannelinput.h"

namespace Ui {
class ChannelStripWidget;
}

class ChannelStripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChannelStripWidget(MixerChannel* channel, QWidget *parent = 0);
    virtual ~ChannelStripWidget();

    MixerChannel* GetChannel() { return this->_channel; }

public slots:
    void OnCloseClicked();
    void SetChannelColor(const QColor& color);

    void OnInstrumentClicked();
    void InstrumentPicked(QAction* action);
    void ChangeChannelInput(MixerChannelInput* generator);

private:
    Ui::ChannelStripWidget *ui;
    class SendButtonStrip* ui_sendButtonStrip;
    class EffectButtonStrip* ui_effectButtonStrip;
    MixerChannel* _channel;

    bool eventFilter(QObject* watched, QEvent* event);
    void PickInstrument(const QPoint &pos);
};

#endif // CHANNELSTRIPWIDGET_H
