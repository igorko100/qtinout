#include <QTimer>

#include "showlasteventdialog.h"
#include "ui_showlasteventdialog.h"

ShowLastEventDialog::ShowLastEventDialog(QString &pib, QString &status, QString &timestamp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLastEventDialog)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color:white;");

    if(pib != "")
        ui->PibLabel->setText(pib);
    else
        ui->PibLabel->setText("No name found. Update the DB, please.");
//    ui->timestampLabel->setText(timestamp);

    QString date = timestamp.mid(0, 10);
    QString time = timestamp.mid(10, -1);

    if(status == "in") {
        ui->statusLabel->setText(QString("на робочому місці з: ") + time + "       Дата: " + date);
        ui->labelForPixmap->setPixmap(QPixmap("at_work.png"));
    }
    else {
        ui->statusLabel->setText(QString("вийшов з роботи у:") + time + "       Дата: " + date);
        ui->labelForPixmap->setPixmap(QPixmap("at_home.png"));
    }

    QTimer::singleShot(5000, this, SLOT(close()));

}

ShowLastEventDialog::~ShowLastEventDialog()
{
    delete ui;
}
