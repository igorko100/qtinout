#ifndef ADDNEWUSERDIALOG_H
#define ADDNEWUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>

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

    bool currentInput;


protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    void digitClicked();

};

#endif // ADDNEWUSERDIALOG_H
