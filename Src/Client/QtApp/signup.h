#ifndef SIGNUP_H
#define SIGNUP_H
#include <QDialog>
#include <QtCore>
#include <QtGui>
//#include "clientm.h"
namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = 0);
    char* theUserName;
    char* globPort;
    char* globIp;
    ~SignUp();

private slots:


    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


private:
    Ui::SignUp *ui;
    QStandardItemModel *model;
};

#endif // SIGNUP_H
