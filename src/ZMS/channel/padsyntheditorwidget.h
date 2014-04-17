#ifndef PADSYNTHEDITORWIDGET_H
#define PADSYNTHEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class PadSynthEditorWidget;
}

class PadSynthEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PadSynthEditorWidget(QWidget *parent = 0);
    ~PadSynthEditorWidget();

private:
    Ui::PadSynthEditorWidget *ui;
};

#endif // PADSYNTHEDITORWIDGET_H
