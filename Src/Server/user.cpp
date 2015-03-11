#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <uuid/uuid.h>
#include <unistd.h>
#include <cstdlib>
//#include "user.h"
using namespace std;

/* Note compile with -luuid -std==c++11 */
//assumed that password was correct

struct fileInfo
{
	string name;
	string path;
	int version;
	string alias; 
};


struct clientData
{
	string name;
	string path;
	int version;
};


//typedef string clientString;

class pitara_user
{
	private:
		string user_id;
		string serverTxt;
		fstream serverFile;
		unordered_map<string,string> fileData;
		string genServerAlias(clientData data);
		fileInfo buildFileStr(string fileLine);
		clientData getClientData(fileInfo file);
		bool compareCD(clientData a,clientData b);
	public:
		pitara_user(string user);
		void Absorb(); 
		string getClientString(clientData file);
		clientData makeClientData(string name,string path,int version);
		//void deployFileData();
		string AddFile(clientData file);
		//void ModifyFile(clientData file);
		void removeFile(clientData file);
		void shareFile(clientData file,string friend_name);
		string downloadFile(clientData file);		
		string getUserData();
		//void AddFile(fstream file, )
		

};


pitara_user::pitara_user(string user){
	user_id=user;
	string temp;
	temp=user_id+".txt";
	serverTxt=temp;
	//serverFile.open(temp,ios::out);
	//serverFile.close();
}

string pitara_user::genServerAlias(clientData data){
	string temp;
	uuid_t uuid;
    uuid_generate_random(uuid);
    char s[37];
    uuid_unparse ( uuid, s );
	temp=data.name+"?"+data.path+"?"+string(to_string(data.version))+"?"+string(s)+"-"+string(to_string(data.version))+'-'+data.name;
	return temp;
}


fileInfo pitara_user::buildFileStr(string fileLine)
{
	size_t ques=fileLine.find("?");
	fileInfo temp;
	temp.name=fileLine.substr(0,ques);
	fileLine=fileLine.substr(ques+1,fileLine.length());
	ques=fileLine.find("?");
	temp.path=fileLine.substr(0,ques);
	fileLine=fileLine.substr(ques+1,fileLine.length());
	ques=fileLine.find("?");
	temp.version=stoi(fileLine.substr(0,ques));
	fileLine=fileLine.substr(ques+1,fileLine.length());
	ques=fileLine.find("?");
	temp.alias=fileLine.substr(0,ques);
	fileLine=fileLine.substr(ques+1,fileLine.length());
	return temp;
}

clientData pitara_user::getClientData(fileInfo file){
	clientData temp;
	temp.name=file.name;
	temp.path=file.path;
	temp.version=file.version;
	return temp;

}

string  pitara_user::getUserData(){
	string line;
	string data;
	serverFile.open(serverTxt,ios::in);
	if (serverFile.is_open()){
		while(getline(serverFile,line)){
			data.append(line+"\n");
		}
	serverFile.close();
	if(data=="") return "NoData";
	return data;
	}
	else{
		return "NoData";
	}
}

clientData pitara_user::makeClientData(string name,string path, int version){
	clientData temp;
	temp.name=name;
	temp.path=path;
	temp.version=version;
	return temp;
}

string pitara_user::getClientString(clientData file){
	string temp;
	temp=file.name+"?"+file.path+"?"+string(to_string(file.version));
	return temp;
}
void pitara_user::Absorb(){
	fileData.clear();
	string line;
	serverFile.open(serverTxt,ios::in);
	if (serverFile.is_open()){
		while(getline(serverFile,line)){
			fileInfo temp;
			clientData temp2;
			temp=buildFileStr(line);
			temp2=getClientData(temp);
			string temp3=getClientString(temp2);
			fileData.insert(make_pair(temp3,temp.alias));
		}
		serverFile.close();
	}
}

string pitara_user::AddFile(clientData file){

	serverFile.open(serverTxt,ios::out|ios::app);
	if (serverFile.is_open()){
		string temp=genServerAlias(file);
		serverFile<<temp<<endl;
		serverFile.close();
		Absorb();
		string temp2= getClientString(file);
		string temp3=fileData.at(temp2);
		return temp3;	
	}
	else
		return "";
}

string pitara_user::downloadFile(clientData file){
	Absorb();
	string temp1= getClientString(file);
	cout<<"the client string is: "<<temp1<<endl;
	string temp=fileData.at(temp1);
	return temp;
}

void pitara_user::shareFile(clientData file,string friend_name){
	Absorb();
	string h=getClientString(file);
	string temp=fileData.at(h);
	string nOpen=friend_name+".txt";
	fstream tempFile;
	tempFile.open(nOpen,ios::out|ios::app);
	cout<<"received username: "<<friend_name<<endl;
	if(tempFile.is_open()){
		string sendFile=file.name+"?"+"/home/Pitara/MyDownloads/"+file.name+"?"+string(to_string(file.version))+"?"+temp+"\n";
		tempFile << sendFile;
		tempFile.close();
	}

}


bool pitara_user::compareCD(clientData a,clientData b){
	if ((a.name==b.name) && (a.path==b.path)){
		return true;
	}
	else{
		return false;
	}
}


void pitara_user::removeFile(clientData file){
	string line;
	string data;
	string retVal;
	serverFile.open(serverTxt,ios::in);
	if (serverFile.is_open()){
		while(getline(serverFile,line)){
			fileInfo temp;
			clientData temp2;
			temp=buildFileStr(line);
			temp2=getClientData(temp);
			if (!compareCD(file,temp2)){
				data.append(line+"\n");
			}
			else{
				string fileName=temp.alias;

				string command="rm "+fileName;
				system(command.c_str());
			}

		}
	serverFile.close();
	//sleep(1);
	}
	serverFile.open(serverTxt,ios::out);
	if(serverFile.is_open()) {
		serverFile << data ;
		serverFile.close();
	}

}	


/*int main() {
	
	pitara_user aayush("cs1130201");
	clientData temp1;
	clientData temp2;
	clientData temp3;
	clientData temp4;
	clientData temp5;
	temp1=aayush.makeClientData("abc1.txt","/home/Pitara/abc1.txt",1);
	temp2=aayush.makeClientData("abc2.txt","/home/Pitara/abc2.txt",3);
	temp3=aayush.makeClientData("abc3.txt","/home/Pitara/abc3.txt",1);
	temp4=aayush.makeClientData("abc4.txt","/home/Pitara/abc4.txt",5);
	temp5=aayush.makeClientData("abc5.txt","/home/Pitara/abc5.txt",1);
	aayush.AddFile(temp1);
	aayush.AddFile(temp2);
	aayush.AddFile(temp3);
	aayush.AddFile(temp4);
	aayush.AddFile(temp5);
	aayush.shareFile(temp2,"cs1130204");
	aayush.removeFile(temp3);
	string a=aayush.downloadFile(temp1);
	cout<< "downloaded file name :" << a << endl;
}*/
