#include <QPushButton>

#include <QDebug>

#include "addnewuserdialog.h"
#include "ui_addnewuserdialog.h"

AddNewUserDialog::AddNewUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewUserDialog)
{
    ui->setupUi(this);

    currentInput = 0; // Input card number

//    currentInput = ui->cardNumberLineEdit;

    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digitClicked()));
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digitClicked()));


    ui->cardNumberLineEdit->installEventFilter(this);
    ui->passwordLineEdit->installEventFilter(this);

}

AddNewUserDialog::~AddNewUserDialog()
{
    delete ui;
}


bool AddNewUserDialog::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->cardNumberLineEdit && event->type() == QEvent::FocusIn) {
        currentInput = 0;

        qDebug()<<"in eventFilter  0!!!";

        return false; // lets the event continue to the edit
    }

    if(object == ui->passwordLineEdit && event->type() == QEvent::FocusIn) {
                currentInput = 1;

                qDebug()<<"in eventFilter  1!!!";


                return false; // lets the event continue to the edit
            }

    return false;
}


void AddNewUserDialog::digitClicked() {
    {
        QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
        int digitValue = clickedButton->text().toInt();
        if (currentInput == 0)
            ui->cardNumberLineEdit->setText(ui->cardNumberLineEdit->text() + QString::number(digitValue));
        else
            ui->passwordLineEdit->setText(ui->passwordLineEdit->text() + QString::number(digitValue));
    }
}

