#ifndef INSERTEFFECTBUTTONSTRIP_H
#define INSERTEFFECTBUTTONSTRIP_H

#include <QWidget>
#include <QPushButton>
#include <QDial>
#include <QAction>
#include <QHBoxLayout>
#include "mixer/mixerinserteffect.h"
#include "mixer/mixerinserteffectcontainer.h"

namespace Ui {
class InsertEffectButtonStrip;
}

class InsertEffectButton : public QWidget
{
    Q_OBJECT

public:
    InsertEffectButton(MixerInsertEffect* effect, QWidget* parent = 0);
    virtual ~InsertEffectButton();

    MixerInsertEffect* Effect() { return this->_effect; }

signals:
    void setText(const QString& text);

private:
    QHBoxLayout* ui_layout;
    QPushButton* ui_button;
    QDial* ui_dial;
    MixerInsertEffect* _effect;

    virtual bool eventFilter(QObject *o, QEvent *e);

};

class InsertEffectButtonStrip : public QWidget
{
    Q_OBJECT

public:
    InsertEffectButtonStrip(MixerInsertEffectContainer* effects, QWidget *parent = 0);
    virtual ~InsertEffectButtonStrip();

public slots:
    void OnAddEffectClicked();
    void AddEffect(MixerInsertEffect* effect);
    void RemoveEffect(MixerInsertEffect* effect);
    void ActionSelected(QAction* action);

private:
    Ui::InsertEffectButtonStrip *ui;
    MixerInsertEffectContainer* _effects;
    QAction* _removeAction;
    QAction* _editAction;

    virtual bool eventFilter(QObject *o, QEvent *e);
};

#endif // INSERTEFFECTBUTTONSTRIP_H
