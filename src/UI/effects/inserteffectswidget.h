#ifndef INSERTEFFECTSWIDGET_H
#define INSERTEFFECTSWIDGET_H

#include <QWidget>

namespace Ui {
class InsertEffectsWidget;
}

class MixerEffect;

class InsertEffectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InsertEffectsWidget(QWidget *parent = 0);
    ~InsertEffectsWidget();

public slots:
    void OnNewEffectClicked();
    void OnCloseEffectClicked(int index);
    void AddInsertEffect(MixerEffect* effect);
    void RemoveInsertEffect(MixerEffect* effect);
    void SelectEffect(MixerEffect* effect);

private:
    Ui::InsertEffectsWidget *ui;
};

#endif // INSERTEFFECTSWIDGET_H
