#include "showlasteventdialog.h"
#include "ui_showlasteventdialog.h"

ShowLastEventDialog::ShowLastEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLastEventDialog)
{
    ui->setupUi(this);
}

ShowLastEventDialog::~ShowLastEventDialog()
{
    delete ui;
}
