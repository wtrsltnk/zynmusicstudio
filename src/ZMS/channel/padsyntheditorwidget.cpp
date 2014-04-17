#include "padsyntheditorwidget.h"
#include "ui_padsyntheditorwidget.h"

PadSynthEditorWidget::PadSynthEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PadSynthEditorWidget)
{
    ui->setupUi(this);
}

PadSynthEditorWidget::~PadSynthEditorWidget()
{
    delete ui;
}
