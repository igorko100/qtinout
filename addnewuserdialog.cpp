#include <QPushButton>

#include <QDebug>

#include "addnewuserdialog.h"
#include "ui_addnewuserdialog.h"

/*
When compiled on PC - there is a problem (when choosing LineEdit with focus...)
*** stack smashing detected ***: <unknown> terminated
The program has unexpectedly finished.

It never shows up on Tinker board though.
*/


AddNewUserDialog::AddNewUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewUserDialog)
{
    ui->setupUi(this);

    currentInput = 0; // Input card number
    pass = QString("12345678");

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

    connect(ui->createNewUserButton, SIGNAL(clicked()), this, SLOT(createNewUserClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->backSpaceButton, SIGNAL(clicked()), this, SLOT(backSpace()));

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
        return false; // lets the event continue to the edit
    }

    if(object == ui->passwordLineEdit && event->type() == QEvent::FocusIn) {
        currentInput = 1;
        return false; // lets the event continue to the edit
    }

    return false;
}

void AddNewUserDialog::backSpace() {
    if (currentInput == 0) {
        ui->cardNumberLineEdit->backspace();
        ui->cardNumberLineEdit->setFocus();
    }
    else {
        ui->passwordLineEdit->backspace();
        ui->passwordLineEdit->setFocus();
    }
}

int AddNewUserDialog::getCardNumber() {
    return ui->cardNumberLineEdit->text().toInt();
}

void AddNewUserDialog::createNewUserClicked() {
    if(ui->passwordLineEdit->text() == pass) {
        qDebug()<<"Create accepted!!!";
        emit accepted();
        accept();
    }
}

void AddNewUserDialog::digitClicked() {
    {
        QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
        int digitValue = clickedButton->text().toInt();
        if (currentInput == 0) {
            ui->cardNumberLineEdit->setText(ui->cardNumberLineEdit->text() + QString::number(digitValue));
            ui->cardNumberLineEdit->setFocus();
        }
        else {
            ui->passwordLineEdit->setText(ui->passwordLineEdit->text() + QString::number(digitValue));
            ui->passwordLineEdit->setFocus();
        }
    }
}

