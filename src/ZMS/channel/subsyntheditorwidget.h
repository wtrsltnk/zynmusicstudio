#ifndef SUBSYNTHEDITORWIDGET_H
#define SUBSYNTHEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class SubSynthEditorWidget;
}

class SubSynthEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubSynthEditorWidget(QWidget *parent = 0);
    ~SubSynthEditorWidget();

private:
    Ui::SubSynthEditorWidget *ui;
};

#endif // SUBSYNTHEDITORWIDGET_H
