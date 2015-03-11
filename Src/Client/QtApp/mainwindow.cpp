#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "clientm.cpp"
#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText(QString("User Name"));
    ui->lineEdit_2->setPlaceholderText(QString("Password"));
}

MainWindow::~MainWindow()
{
    delete ui;
    qApp->setStyleSheet("QLineEdit { background-color: yellow }");
}

void MainWindow::on_pushButton_clicked()
{
    QString username,password;
    username=ui->lineEdit->text();
    password=ui->lineEdit_2->text();
    QByteArray ba1 = username.toLocal8Bit();
    char* user = ba1.data();
    QByteArray ba2 = password.toLocal8Bit();
    char* pass = ba2.data();
    string shallLogin= main2(globIp, globPort, "login",user, pass, "","", "",0,"");

    if(shallLogin=="ok"){
        this->hide();
        UserInfo userPage;
        userPage.theUserName=user;
        userPage.globIp=globIp;
        userPage.globPort=globPort;
        userPage.setModal(true);
        userPage.exec();
    }
    else{
        ui->label_6->setText("Incorrect username and password");
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    signUp = new SignUp(this);
    signUp->globIp=globIp;
    signUp->globPort=globPort;
    signUp->show();
}



void MainWindow::on_commandLinkButton_clicked()
{
    this->hide();
    ForgotPassword forgot;
    forgot.globIp=globIp;
    forgot.globPort=globPort;
    forgot.setModal(true);
    forgot.exec();
}
