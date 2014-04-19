#ifndef CHANNELSTRIPCONTAINER_H
#define CHANNELSTRIPCONTAINER_H

#include <QWidget>
#include "channelstripwidget.h"

namespace Ui {
class ChannelStripContainer;
}

class ChannelStripContainer : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelStripContainer(QWidget *parent = 0);
    ~ChannelStripContainer();

    ChannelStripWidget* GetWidgetByChannel(Instrument* channel);

public slots:
    void UpdateChannels();
    void moveScrollBarToBottom(int min, int max);
    void AddChannel();

private:
    Ui::ChannelStripContainer *ui;
    int _lastMax;

    friend class MainWindow;
    MainWindow* _mainParent;
};

#endif // CHANNELSTRIPCONTAINER_H
