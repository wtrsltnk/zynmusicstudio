#ifndef ADDSYNTHEDITORWIDGET_H
#define ADDSYNTHEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class AddSynthEditorWidget;
}

class AddSynthEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddSynthEditorWidget(QWidget *parent = 0);
    ~AddSynthEditorWidget();

private:
    Ui::AddSynthEditorWidget *ui;
};

#endif // ADDSYNTHEDITORWIDGET_H
