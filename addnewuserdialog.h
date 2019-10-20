#ifndef ADDNEWUSERDIALOG_H
#define ADDNEWUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QString>

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

    QString pass;


protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    void digitClicked();
    void createNewUserClicked();

};

#endif // ADDNEWUSERDIALOG_H
