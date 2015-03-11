#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <QFile>
#include <QString>
#include "clientm.h"
#include <QTextStream>
#include "clientFolder.h"
#include <cstdlib>
using namespace std;


class myfile{
public:
    string filename;
    string path;
    string modifyDate;
    int versions=1;
};
ofstream destination;
vector<myfile> allFiles; //all the new info files of user
vector<myfile> oldFiles; //the previous information extracted from the txt file

ifstream oldfileinfos;
ofstream onserver;
using namespace std;

/*function... might want it in some class?*/
int getdir(string dir)
{

    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
       // cout<<"didn't work out!!"<<endl;
        exit(0);
    }

    while ((dirp = readdir(dp)) != NULL) {
        myfile f;
        struct stat sb;
        string newdir= dir+"/"+string(dirp->d_name);
        int chk=stat(newdir.c_str(),&sb);
        if(chk==-1){
            //cout<<"this dir: "<<dir<<" this name: "<<dirp->d_name<<endl;
            perror("stat");
            //cout<<"this one error"<<endl;
            exit(EXIT_FAILURE);
        }
        string name = string(dirp->d_name);

        switch (sb.st_mode & S_IFMT) {

        case S_IFDIR:
                    if(name[0]!='.'){
                                //cout<<"oh a directory: "<<name<<endl;
                                string newdir = dir+"/"+string(dirp->d_name);
                                getdir(newdir);
                    }

                    break;
        case S_IFREG:
                    if(name[0]!='.'){
                            //cout<<"oh a file: "<<name<<endl;
                            f.filename= name;
                            f.path=dir+"/"+string(dirp->d_name);
                            f.modifyDate=ctime(&sb.st_mtime);
                            f.modifyDate=f.modifyDate.substr(0,f.modifyDate.length()-1);
                            allFiles.push_back(f);
                    }


                     break;
        default:       printf("unknown?\n");                break;
         }
    }
    closedir(dp);
    return 0;
}


void tellmeUpdated(char* username,char* Ip,char* Port){
    for(int i=0;i<allFiles.size();i++){
        bool found=false;
        myfile newf=allFiles[i];
        for(int j=0;j<oldFiles.size();j++){
            myfile old=oldFiles[j];
            if(newf.path==old.path){
                found=true;
                if(newf.modifyDate!=old.modifyDate){
                    //cout<<"new mod date: "<<newf.modifyDate<<" old mod date: "<<old.modifyDate<<endl;
                    /*Store newf in the onserverclient database*/
                    /*update the number of versions specifically*/
                    allFiles[i].versions=oldFiles[j].versions+1;
                    //upload it on the server
                    //cout<<"uploading the file: "<<newf.filename<<endl;
                    //delete the file from oldFiles to find the deleted ones
                    char sendPath[newf.path.length()];// = new char(newf.path.length());
                    sprintf(sendPath,newf.path.c_str());
                    //cout<<"sending path: "<<sendPath<<endl;
                    char sendname[newf.filename.length()];//= new char(newf.filename.length());
                    sprintf(sendname,newf.filename.c_str());
                    //cout<<"seding name: "<<sendname<<endl;
                    //updateOnServerClientdata(newf,"modify");
                    //main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filepath, int versions)
                    main2(Ip,Port,"upload",username,"","",sendname, sendPath,allFiles[i].versions,"");
                   // delete[] sendPath;
                   // delete[] sendname;
                }
                oldFiles.erase(oldFiles.begin()+j);
            }
        }
        if(found==false){
            //upload it on the server
            /*add an entry in the onserverclient database*/
            //cout<<"uploading the file: "<<newf.filename<<endl;
            //delete the file from oldFiles to find the deleted ones
            char sendPath[newf.path.length()];// = new char(newf.path.length());
            sprintf(sendPath,newf.path.c_str());
            char sendname[newf.filename.length()];//= new char(newf.filename.length());
            sprintf(sendname,newf.filename.c_str());
            //updateOnServerClientdata(newf,"add");
            main2(Ip,Port,"upload",username,"","",sendname,sendPath,allFiles[i].versions,"");
            //delete[] sendPath;
            //delete[] sendname;
        }
    }
    //remaining are the deleted ones by the user
    //send information to the server for the deleted ones
    string finalfiles;
    for(int i=0;i<oldFiles.size();i++){
        //delete them from the onserverclient database
        QString s = QString::number(oldFiles[i].versions);
        //string a = s.data();
        string a = s.toUtf8().constData();
        //cout<<"WritingDate: "<<oldFiles[i].modifyDate<<endl;
        string filesinfo=oldFiles[i].filename+"?"+ oldFiles[i].path+"?"+oldFiles[i].modifyDate+"?"+a;
               finalfiles+=filesinfo;
    }
    if(finalfiles.length()!=0)finalfiles=finalfiles.substr(0,finalfiles.length()-1);
        //cout<<"deleted ones: \n"<<finalfiles<<endl;
}
void getTheOldFiles(){
    ifstream ifs("clientfiles.txt");
    string alloldinfo;
    string token="";
    alloldinfo.assign((istreambuf_iterator<char>(ifs)),
                      (istreambuf_iterator<char>()));
    alloldinfo+='\n';

    int count=1;
    int iter=0;
    string thename="";
    string thepath="";
    string thetime="";
    //int theVersion=1;
    if(ifs.is_open()){
    while(iter<alloldinfo.length()){
        //if(alloldinfo[iter]!='\n'){
            if(alloldinfo[iter]=='?'){
                if(count==1){
                    //cout<<"name: "<<token<<endl;
                    thename=token;
                    token="";
                    count++;
                }
                else if(count==2){
                   // cout<<"path: "<<token<<endl;
                    thepath=token;
                    token="";
                    count++;
                }
                else if(count==3){
                    //cout<<"time: "<<token<<endl;
                    thetime=token;
                    token="";
                    count++;
                }
            }
            else if(alloldinfo[iter]=='\n'){
               // cout<<"version: "<<token<<endl;
                QString tok=QString::fromStdString(token);
                token="";
                count=1;
                bool ok;
                myfile f;
                f.filename=thename;
                f.path=thepath;
                f.modifyDate=thetime;
                f.versions=tok.toInt(&ok,10);
                oldFiles.push_back(f);
            }
            else{
                token+=alloldinfo[iter];
            }
                            iter++;
        //}
        //else{
       // 	iter++;
       // 	count=1;
       // 	token="";
        //}
        ifs.close();
    }
    }
    else{
        //cout<<"couldn't read the clientfiles.txt<<endl";
        return;
    }

}

int gatherTheFiles(char* username,char* Ip,char* Port)
{
    string dir = "./clientfolder/MyPitara";
    //vector<string> files = vector<string>();
    getdir(dir);//all the files in the directory with their name,pathname and last modified date, initially versions are kept 1
    getTheOldFiles();//loads the old files vecoter with the previous states
    tellmeUpdated(username,Ip,Port);//compares modified added or deleted and does the respective tasks of uploading or modifying
    QString thefilename = "clientfiles.txt";
    QFile file(thefilename);
    if(file.open(QIODevice::ReadWrite)){
        string finalfiles;
        for (unsigned int i = 0;i < allFiles.size();i++) {
               QString s = QString::number(allFiles[i].versions);
               //string a = s.data();
               string a = s.toUtf8().constData();
               string filesinfo=allFiles[i].filename+"?"+ allFiles[i].path+"?"+allFiles[i].modifyDate+"?"+a+'\n';
               finalfiles+=filesinfo;
        }
        finalfiles=finalfiles.substr(0,finalfiles.length()-1);
        QString finale(finalfiles.c_str());
        QTextStream stream(&file);
        stream << finale;
        file.close();
        finale="";
    }
    allFiles.clear();
    oldFiles.clear();
    return 0;
}

