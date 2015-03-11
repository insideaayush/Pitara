#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QString>
#include <QTreeWidget>
#include <QStandardItemModel>
#include <QFutureWatcher>

//#include "clientm.h"
namespace Ui {
class UserInfo;
}

class UserInfo : public QDialog
{
    Q_OBJECT



public:
    explicit UserInfo(QWidget *parent = 0);
    char* theUserName;
    char* globPort;
    char* globIp;
    ~UserInfo();


private slots:
    //void taskComplete();
    //void completeTheTask();
    void on_quitButton_clicked();
   // QStandardItemModel *model = new QStandardItemModel(0,0,this);
    void on_uploadButton_clicked();

    void on_syncButton_clicked();

    void on_downloadButton_clicked();



    void on_pitaraData_clicked();

    void on_delete_2_clicked();

    void on_shareButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::UserInfo *ui;
    QStandardItemModel *model=new QStandardItemModel(0,0,this);


};

#endif // USERINFO_H
