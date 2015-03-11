#include "userinfo.h"
#include "ui_userinfo.h"
#include "clientFolder.h"
#include "clientm.h"
#include <string>
#include <iostream>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cstring>
#include <Qt>
#include <QtGui>
#include <QImage>
using namespace std;



UserInfo::UserInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfo)
{
    //theClientUserName=theUserName;
    ui->setupUi(this);
    //ui->progressBar->setValue(0);
    //connect(&this->FutureWatcher,SIGNAL(finished()),this,SLOT(taskComplete()));
}

UserInfo::~UserInfo()
{
    delete ui;

}

void UserInfo::on_quitButton_clicked()
{
    exit(0);
}


void UserInfo::on_uploadButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "home/btech/cs1130204",
                "All Files(*.*)"
                );


    QFile f(path);
    QFileInfo fileinfo(f.fileName());
    QString filename(fileinfo.fileName());
   QByteArray ba1 = path.toLocal8Bit();
   QByteArray ba2 = filename.toLocal8Bit();
   char* fullPath = ba1.data();
   char* onlyName = ba2.data();

    //cout<<"upload is pressed"<<endl;
    //cout<<sizeof(fullPath);
    if(path.length()!=0){
        //cout<<"oh file selected!"<<endl;
        QMessageBox::information(this,tr("File Name"),path);
        main2(globIp, globPort,"upload",theUserName, "", "",onlyName, fullPath,1,"");
    }
    //cout<<"trying"<<endl;
}


void UserInfo::on_syncButton_clicked()
{
    //cout<<"clicked sync"<<endl;
   // model->setItem(1,1,new QStandardItem(QString(thename)));
   // cout<<"clicked sync"<<endl;
    //QFuture<void> future = QtConcurrent::run(&this->)
    ui->label_3->setText("Please wait, sync in progress.");
    gatherTheFiles(theUserName,globIp,globPort);
    ui->label_3->setText("Sync Complete.");
    this->on_pitaraData_clicked();
}

void UserInfo::on_downloadButton_clicked()
{
    //theClientUserName=theUserName;
    //cout<<"clicked download"<<endl;

    QModelIndex selection= ui->treeView->currentIndex();
    int row = selection.row();
    QModelIndex fileName=model->index(row,0,QModelIndex());
    QModelIndex filePath=model->index(row,1,QModelIndex());
    QModelIndex fileVersion=model->index(row,2,QModelIndex());
    QVariant value= model->data(fileName,Qt::DisplayRole);
    QVariant value1= model->data(filePath,Qt::DisplayRole);
    QVariant value2= model->data(fileVersion,Qt::DisplayRole);
    QString thename=value.toString();
    QString thepath=value1.toString();
    QString theversion=value2.toString();


   //mess=ui->pathName->toPlainText();
    QByteArray ba1 = thename.toLocal8Bit();
    QByteArray ba2 = thepath.toLocal8Bit();
    QByteArray ba3 = theversion.toLocal8Bit();
    char* selectedFileName=ba1.data();
    char* selectedFilePath=ba2.data();
    char* selectedFileVersion=ba3.data();
    //cout<<"filename "<<selectedFileName<<" file path "<<selectedFilePath<<" file version "<<selectedFileVersion<<endl;
    int version=theversion.toInt();
    //main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filepath, int version,char* secans)
    //int versionToDown =1;
    main2(globIp, globPort,"download",theUserName, "", "",selectedFileName, selectedFilePath,version,"");
}



void UserInfo::on_pitaraData_clicked()
{
    model->clear();
    string info=main2(globIp, globPort,"showmyFiles",theUserName, "", "","", "",1,"");
    cout<<"received info" <<info<<endl;
    if(info=="NoData"){
        QMessageBox::information(this,tr("No data yet!"),"Please upload or sync some data first");
    }
    //theClientUserName=theUserName;

    //QStandardItemModel *model = new QStandardItemModel(20,3,this); //2 Rows and 3 Columns
    //model = new QStandardItemModel(0,0,this); //2 Rows and 3 Columns
    //model->layoutChanged();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Filename")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("FilePath")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Version Number")));
    //QStandardItem *first = new QStandardItem(QString("ColumnValue"));
    //ui->tableView->verticalHeader()->setVisible(false);
    //model->setItem(0,0,first);
    //ui->tableView->setModel(model);
    ui->treeView->setModel(model);
    //model->setParent();
    int count = 1;
    int iter =0;
    QString token ="";
    QString thename ="";
    QString thepath = "";
    QString thetime = "";
    QString alloldinfo;
        int row =0;
        int column =0;
        int theVersion;
        QStandardItem *mainParent = model->invisibleRootItem();
        QStandardItem *newParent;
        alloldinfo=QString::fromStdString( info);
        cout<<"while loop starting"<<endl;
    while(iter<alloldinfo.length()){
                if(alloldinfo[iter]=='?'){
                    if(count==1){
                        //cout<<"token: "<<endl;
                        cout<<token.toStdString()<<endl;
                        thename=token;
                        //cout<<"thename is"<<thename.toStdString()<<endl;
                        QStandardItem *changeName=new QStandardItem(thename);
                        mainParent->appendRow(changeName);
                        newParent=changeName;
                        token="";
                        count++;
                        column++;
                    }
                    else if(count==2){
                        //cout<<"token: "<<token<<endl;
                        thepath=token;
                        model->setItem(row,column,new QStandardItem(thepath));
                        //cout<<"token:"<<endl;
                        cout<<thepath.toStdString()<<endl;
                        token="";
                        count++;
                        column++;
                    }
                    else if(count==3){
                        //cout<<"token: "<<token<<endl;
                        thetime=token;
                        model->setItem(row,column,new QStandardItem(thetime));
                        //cout<<"token:"<<endl;
                        cout<<thetime.toStdString()<<endl;
                        token="";
                        count++;
                        column++;
                    }
                }
                else if(alloldinfo[iter]=='\n'){
                    count=1;
                    bool ok;
                    theVersion=token.toInt(&ok,10);
                    for(int i=1;i<=theVersion;i++){
                        QString s="Version: ";
                        s += QString::number(i);
                        QStandardItem *versionItems = new QStandardItem(s);
                        newParent->appendRow(versionItems);
                    }

                    token="";
                    column=0;
                    row++;
                }
                else{

                    token+=alloldinfo[iter];
                }
                                iter++;
        }
}

void UserInfo::on_delete_2_clicked()
{
    //cout<<"clicked delete"<<endl;
    QModelIndex selection= ui->treeView->currentIndex();
    int row = selection.row();
    QModelIndex fileName=model->index(row,0,QModelIndex());
    QModelIndex filePath=model->index(row,1,QModelIndex());
    QModelIndex fileVersion=model->index(row,2,QModelIndex());
    QVariant value= model->data(fileName,Qt::DisplayRole);
    QVariant value1= model->data(filePath,Qt::DisplayRole);
    QVariant value2= model->data(fileVersion,Qt::DisplayRole);
    QString thename=value.toString();
    QString thepath=value1.toString();
    QString theversion=value2.toString();
    QByteArray ba1 = thename.toLocal8Bit();
    QByteArray ba2 = thepath.toLocal8Bit();
    QByteArray ba3 = theversion.toLocal8Bit();
    char* selectedFileName=ba1.data();
    char* selectedFilePath=ba2.data();
    char* selectedFileVersion=ba3.data();
    //cout<<"filename "<<selectedFileName<<" file path "<<selectedFilePath<<" file version "<<selectedFileVersion<<endl;
    int version=theversion.toInt();
    //main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filepath, int version,char* secans)
    //int versionToDown =1;
    main2(globIp, globPort,"deleteFromServer",theUserName, "", "",selectedFileName, selectedFilePath,1,"");
    this->on_pitaraData_clicked();
}

void UserInfo::on_shareButton_clicked()
{
    //theClientUserName=theUserName;
   // cout<<"clicked download"<<endl;
    //char* fileToDownload="file3.pdf";
    //char* filePath="./clientfolder/MyPitara/file3.pdf";
    QModelIndex selection= ui->treeView->currentIndex();
    int row = selection.row();
    QModelIndex fileName=model->index(row,0,QModelIndex());
    QModelIndex filePath=model->index(row,1,QModelIndex());
    QModelIndex fileVersion=model->index(row,2,QModelIndex());
    QVariant value= model->data(fileName,Qt::DisplayRole);
    QVariant value1= model->data(filePath,Qt::DisplayRole);
    QVariant value2= model->data(fileVersion,Qt::DisplayRole);
    QString thename=value.toString();
    QString thepath=value1.toString();
    QString theversion=value2.toString();

    QString shareWith;
    shareWith=ui->shareName->text();
    QByteArray ba4 = shareWith.toLocal8Bit();
    char* finalShare=ba4.data();
    if(shareWith.length()==0){
        QMessageBox::information(this,tr("Invalid Request!"),"Please enter a username to sharewith");
        return;
    }
   //mess=ui->pathName->toPlainText();
    QByteArray ba1 = thename.toLocal8Bit();
    QByteArray ba2 = thepath.toLocal8Bit();
    QByteArray ba3 = theversion.toLocal8Bit();
    char* selectedFileName=ba1.data();
    char* selectedFilePath=ba2.data();
    char* selectedFileVersion=ba3.data();
    //cout<<"filename "<<selectedFileName<<" file path "<<selectedFilePath<<" file version "<<selectedFileVersion<<endl;
    int version=theversion.toInt();
    //main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filepath, int version,char* secans)
    //int versionToDown =1;
    main2(globIp, globPort,"share",theUserName, "", finalShare,selectedFileName, selectedFilePath,version,"");
}

void UserInfo::on_treeView_clicked(const QModelIndex &index)
{
    QModelIndex selection= ui->treeView->currentIndex();
    int row = selection.row();
    QModelIndex fileName=model->index(row,0,QModelIndex());
    QModelIndex filePath=model->index(row,1,QModelIndex());
    QModelIndex fileVersion=model->index(row,2,QModelIndex());
    QVariant value= model->data(fileName,Qt::DisplayRole);
    QVariant value1= model->data(filePath,Qt::DisplayRole);
    QVariant value2= model->data(fileVersion,Qt::DisplayRole);
    QString thename=value.toString();
    QString thepath=value1.toString();
    QString theversion=value2.toString();


   //mess=ui->pathName->toPlainText();
    QByteArray ba1 = thename.toLocal8Bit();
    QByteArray ba2 = thepath.toLocal8Bit();
    QByteArray ba3 = theversion.toLocal8Bit();
    char* selectedFileName=ba1.data();
    char* selectedFilePath=ba2.data();
    char* selectedFileVersion=ba3.data();
    //cout<<"filename "<<selectedFileName<<" file path "<<selectedFilePath<<" file version "<<selectedFileVersion<<endl;



}
