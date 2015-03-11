#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include <QDialog>
#include "clientm.h"

namespace Ui {
class ForgotPassword;
}

class ForgotPassword : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPassword(QWidget *parent = 0);
    char* globPort;
    char* globIp;
    char* theUserName;
    ~ForgotPassword();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ForgotPassword *ui;
};

#endif // FORGOTPASSWORD_H
