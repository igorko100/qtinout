#include <QTimer>

#include "showlasteventdialog.h"
#include "ui_showlasteventdialog.h"

ShowLastEventDialog::ShowLastEventDialog(QString &pib, QString &status, QString &timestamp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLastEventDialog)
{
    ui->setupUi(this);

    ui->labelForPixmap->setPixmap(QPixmap("at_work.png"));
    this->setStyleSheet("background-color:white;");


    ui->PibLabel->setText(pib);
    ui->timestampLabel->setText(timestamp);


    if(status == "in")
        ui->statusLabel->setText("is at work since: ");
    else
        ui->statusLabel->setText("went out at:");

    QTimer::singleShot(5000, this, SLOT(close()));

}

ShowLastEventDialog::~ShowLastEventDialog()
{
    delete ui;
}