#include "symbolicnameform.h"
#include "ui_symbolicnameform.h"

SymbolicNameForm::SymbolicNameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolicNameForm)
{
    ui->setupUi(this);
}

SymbolicNameForm::~SymbolicNameForm()
{
    delete ui;
}
