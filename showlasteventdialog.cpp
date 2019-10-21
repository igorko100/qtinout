#include <QTimer>

#include "showlasteventdialog.h"
#include "ui_showlasteventdialog.h"

ShowLastEventDialog::ShowLastEventDialog(QString &pib, QString &status, QString &timestamp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLastEventDialog)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color:white;");


    ui->PibLabel->setText(pib);
//    ui->timestampLabel->setText(timestamp);

    QString date = timestamp.mid(0, 10);
    QString time = timestamp.mid(10, -1);

    if(status == "in") {
        ui->statusLabel->setText(QString("is at work since: ") + time + "       Date: " + date);
        ui->labelForPixmap->setPixmap(QPixmap("at_work.png"));
    }
    else {
        ui->statusLabel->setText(QString("went out at:") + time + "       Date: " + date);
        ui->labelForPixmap->setPixmap(QPixmap("at_home.png"));
    }

    QTimer::singleShot(5000, this, SLOT(close()));

}

ShowLastEventDialog::~ShowLastEventDialog()
{
    delete ui;
}
