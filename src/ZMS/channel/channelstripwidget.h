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
    explicit ChannelStripWidget(QWidget *parent = 0);
    virtual ~ChannelStripWidget();

    MixerChannel* GetChannel() { return this->_channel; }
    void SetChannel(MixerChannel* channel);

public slots:
    void OnCloseClicked();
    void SetChannelColor(const QColor& color);

    void OnInstrumentClicked();
    void InstrumentPicked(QAction* action);
    void ChangeChannelInput(MixerChannelInput* generator);

private:
    Ui::ChannelStripWidget *ui;
    MixerChannel* _channel;

    bool eventFilter(QObject* watched, QEvent* event);
    void PickInstrument(const QPoint &pos);
};

#endif // CHANNELSTRIPWIDGET_H
