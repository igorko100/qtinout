#ifndef SHOWLASTEVENTDIALOG_H
#define SHOWLASTEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowLastEventDialog;
}

class ShowLastEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowLastEventDialog(QWidget *parent = 0);
    ~ShowLastEventDialog();

private:
    Ui::ShowLastEventDialog *ui;
};

#endif // SHOWLASTEVENTDIALOG_H
