#ifndef CHANNELEDITORWIDGET_H
#define CHANNELEDITORWIDGET_H

#include <QWidget>
#include "../Misc/Instrument.h"

namespace Ui {
class ChannelEditorWidget;
}

class ChannelEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelEditorWidget(QWidget *parent = 0);
    ~ChannelEditorWidget();

    Instrument* ChannelObject() { return this->_channel; }
    void SetChannel(Instrument* channel);

protected slots:
    void OnChannelUpdated(Instrument* channel);
    void OnAddSynthEnabledChanged(bool state);
    void OnSubSynthEnabledChanged(bool state);
    void OnPadSynthEnabledChanged(bool state);

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::ChannelEditorWidget *ui;
    Instrument* _channel;
};

#endif // CHANNELEDITORWIDGET_H
