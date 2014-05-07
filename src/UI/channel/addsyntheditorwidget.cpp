#include "addsyntheditorwidget.h"
#include "ui_addsyntheditorwidget.h"

AddSynthEditorWidget::AddSynthEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSynthEditorWidget)
{
    ui->setupUi(this);
}

AddSynthEditorWidget::~AddSynthEditorWidget()
{
    delete ui;
}
