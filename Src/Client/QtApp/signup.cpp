#include "signup.h"
#include "ui_signup.h"
#include "userinfo.h"
#include "clientm.h"
#include <QMessageBox>
SignUp::SignUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->secQAns->setPlaceholderText(QString("What's your first school's name?"));
}

SignUp::~SignUp()
{
    delete ui;
}


void SignUp::on_pushButton_2_clicked()
{
    this->hide();
}

void SignUp::on_pushButton_clicked()
{
    QString userName,password,securityAns,retypePass;
    userName=ui->username->text();
    password=ui->password->text();
    retypePass=ui->repassword->text();
    securityAns=ui->secQAns->text();
    if(password!=retypePass){
        QMessageBox::information(this,tr("SignUp error"),"Please enter matching passwords");
    }
    else{
        if(password.length()<9){
            QMessageBox::information(this,tr("SignUp error"),"Please enter password of length 9 or more");
        }
        else{
            QByteArray ba2 = password.toLocal8Bit();
            char* finalpass = ba2.data();
            QByteArray ba3 = securityAns.toLocal8Bit();
            char* finalans = ba3.data();

            QByteArray ba1 = userName.toLocal8Bit();

            char* finaluser = ba1.data();
           string test= main2(globIp, globPort,"signup",finaluser, finalpass, "","", "",1,finalans);
           //cout<<"test received"<<test<<endl;
           if(test==""){
                QMessageBox::information(this,tr("connection error"),"Couldn't connect to server");
                return;
           }
           else if(test=="notok"){
               QMessageBox::information(this,tr("UnSuccessful SignUp"),"UserName Already Exists");
               return;
           }
            this->hide();
            UserInfo user;
            user.theUserName=finaluser;
            user.globIp=globIp;
            user.globPort=globPort;
            user.setModal(true);
            user.exec();
        }
    }
}



