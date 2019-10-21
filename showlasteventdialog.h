#ifndef SHOWLASTEVENTDIALOG_H
#define SHOWLASTEVENTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ShowLastEventDialog;
}

class ShowLastEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowLastEventDialog(QString &pib, QString &status, QString &timestamp, QWidget *parent = 0);
    ~ShowLastEventDialog();


private:
    Ui::ShowLastEventDialog *ui;
};

#endif // SHOWLASTEVENTDIALOG_H
