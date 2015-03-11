#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <userinfo.h>
#include <signup.h>
#include <QMainWindow>
#include<QtSql>
#include<QtDebug>
#include<QFileInfo>
#include<QtSql>
#include <forgotpassword.h>
#include "clientm.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    char *theUserName;
    char* globPort;
    char* globIp;
    ~MainWindow();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();


    void on_commandLinkButton_clicked();

private:
    Ui::MainWindow *ui;
    SignUp *signUp;
    QSqlDatabase mydb;
};

#endif // MAINWINDOW_H
