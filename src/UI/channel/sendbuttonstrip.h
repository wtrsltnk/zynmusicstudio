#ifndef SENDBUTTONSTRIP_H
#define SENDBUTTONSTRIP_H

#include <QWidget>
#include <QPushButton>
#include "mixer/mixersendsource.h"

namespace Ui {
class SendButtonStrip;
}

class SendButton : public QPushButton
{
    Q_OBJECT
public:
    SendButton(MixerSendSink* sink);
    virtual ~SendButton();

    MixerSendSink* Sink() { return this->_sink; }

private:
    MixerSendSink* _sink;

};

class SendButtonStrip : public QWidget
{
    Q_OBJECT

public:
    SendButtonStrip(MixerSendSource* sends, QWidget *parent = 0);
    ~SendButtonStrip();

protected slots:
    void OnAddSendClicked();
    void AddSink(MixerSendSink* sink);
    void RemoveSink(MixerSendSink* sink);

private:
    Ui::SendButtonStrip *ui;
    MixerSendSource* _sends;

};

#endif // SENDBUTTONSTRIP_H
