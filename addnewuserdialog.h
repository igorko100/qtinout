#ifndef ADDNEWUSERDIALOG_H
#define ADDNEWUSERDIALOG_H

#include <QDialog>

namespace Ui {
class AddNewUserDialog;
}

class AddNewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewUserDialog(QWidget *parent = 0);
    ~AddNewUserDialog();

private:
    Ui::AddNewUserDialog *ui;
};

#endif // ADDNEWUSERDIALOG_H
