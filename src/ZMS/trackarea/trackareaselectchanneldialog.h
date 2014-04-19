#ifndef TRACKAREASELECTCHANNELDIALOG_H
#define TRACKAREASELECTCHANNELDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "../Misc/Instrument.h"

namespace Ui {
class TrackAreaSelectChannelDialog;
}

class TrackAreaSelectChannelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrackAreaSelectChannelDialog(QWidget *parent = 0);
    ~TrackAreaSelectChannelDialog();

    void SetSelectedChannel(Instrument* channel) { this->_selectedChannel= channel; this->ShowChannels(); }
    Instrument* SelectedChannel() { return this->_selectedChannel; }

protected slots:
    void OnAccepted();
    void OnAddChannel();
    void OnRemoveChannel();

private:
    Ui::TrackAreaSelectChannelDialog *ui;
    Instrument* _selectedChannel;

    void ShowChannels();

};

#endif // TRACKAREASELECTCHANNELDIALOG_H
