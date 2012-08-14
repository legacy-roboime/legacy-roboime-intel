#include "alterstatevars.h"
#include "ui_alterstatevars.h"

AlterStateVars::AlterStateVars(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlterStateVars)
{
    ui->setupUi(this);
}

AlterStateVars::~AlterStateVars()
{
    delete ui;
}
