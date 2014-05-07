#include "subsyntheditorwidget.h"
#include "ui_subsyntheditorwidget.h"

SubSynthEditorWidget::SubSynthEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubSynthEditorWidget)
{
    ui->setupUi(this);
}

SubSynthEditorWidget::~SubSynthEditorWidget()
{
    delete ui;
}
