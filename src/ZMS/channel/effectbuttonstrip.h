#ifndef EFFECTBUTTONSTRIP_H
#define EFFECTBUTTONSTRIP_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include "../Effects/EffectMgr.h"

namespace Ui {
class EffectButtonStrip;
}

class EffectButton : public QPushButton
{
    Q_OBJECT
public:
    EffectButton(EffectMgr* effect);
    virtual ~EffectButton();

    EffectMgr* Effect() { return this->_effect; }

private:
    EffectMgr* _effect;

};

class EffectButtonStrip : public QWidget
{
    Q_OBJECT

public:
    explicit EffectButtonStrip(QWidget *parent = 0);
    ~EffectButtonStrip();

    void UpdateMinHeight();

signals:
    void AddEffectButtonClicked();
    void OnMinimumHeightChanged(int height);

public slots:
    void OnUpdateEffectButtons(QList<EffectMgr*>& effects);
    void OnEffectClicked();
    void OnAddEffectClicked();

private:
    Ui::EffectButtonStrip *ui;

    EffectButton* GetButtonByEffect(EffectMgr* effect);

};

#endif // EFFECTBUTTONSTRIP_H
