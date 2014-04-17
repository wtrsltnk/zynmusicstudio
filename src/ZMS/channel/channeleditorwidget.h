#ifndef CHANNELEDITORWIDGET_H
#define CHANNELEDITORWIDGET_H

#include <QWidget>
#include "../Misc/Channel.h"

namespace Ui {
class ChannelEditorWidget;
}

class ChannelEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelEditorWidget(QWidget *parent = 0);
    ~ChannelEditorWidget();

    Channel* ChannelObject() { return this->_channel; }
    void SetChannel(Channel* channel);

protected slots:
    void OnChannelUpdated(Channel* channel);
    void OnAddSynthEnabledChanged(bool state);
    void OnSubSynthEnabledChanged(bool state);
    void OnPadSynthEnabledChanged(bool state);

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::ChannelEditorWidget *ui;
    Channel* _channel;
};

#endif // CHANNELEDITORWIDGET_H
