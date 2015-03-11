#include "forgotpassword.h"
#include "ui_forgotpassword.h"
#include "userinfo.h"
#include <QMessageBox>
#include <QtCore>

ForgotPassword::ForgotPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgotPassword)
{
    ui->setupUi(this);
    ui->username->setPlaceholderText(QString("Username"));
    ui->securityans->setPlaceholderText(QString("What's your first school's name?"));
}

ForgotPassword::~ForgotPassword()
{
    delete ui;
}

void ForgotPassword::on_pushButton_clicked()
{
    QString forUser=ui->username->text();
    QString theans=ui->securityans->text();
    QByteArray ba1 = forUser.toLocal8Bit();
    QByteArray ba2 = theans.toLocal8Bit();
    char* forgotUser = ba1.data();
    char* knowAns=ba2.data();
    string info=main2(globIp,globPort,"forgotPassword",forgotUser,"","","","",1,knowAns);
    if(info=="notok"){
        QMessageBox::information(this,tr("Something's Wrong"),"The username or security answer is incorrect");
    }
    else{
        QString infoFin=QString::fromStdString(info);
        QMessageBox::information(this,tr("No data yet!"),"Please remember your password:"+infoFin);
        this->hide();
        UserInfo user;
        user.globIp=globIp;
        user.globPort=globPort;
        user.theUserName=forgotUser;
        user.setModal(true);
        user.exec();
    }
}
