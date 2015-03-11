#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
//#include <stdio.h>
#include <regex>

//#include "key.h"
//compile with -std=c++11

using namespace std;

class biomet{
private:
    fstream wallet;
    string userid;
    string pswd;
    string name;
    string quesAns;
    unordered_map<string,string> lock;
    unordered_map<string,string> Safe;

public:
    biomet();
    void loadLock();
    void insertNewProfile(string usr_name,string keyP,string name,string ans);
    bool verifyPaswd(string name, string keyP);
    bool verifyUsr(string usr_name);
    string forgotpassword(string usr_name,string ans);
};


biomet::biomet(){}
void biomet::loadLock(){
	size_t comma;
	size_t length;
	string line;
	lock.clear();
	wallet.open("key.csv",ios::in);
	if (wallet.is_open()){
		while(getline(wallet,line)){
			comma=line.find(",");
			userid=line.substr(0,comma);
			line=line.substr(comma+1,line.length());
			
			comma=line.find(",");
			pswd=line.substr(0,comma);
			line=line.substr(comma+1,line.length());
			
			
			comma=line.find(",");
			name=line.substr(0,comma);
			
			quesAns=line.substr(comma+1,line.length());
			
			
			lock.insert(make_pair(userid,pswd));
			Safe.insert(make_pair(userid,quesAns));

		}
		wallet.close();
	}
}

void biomet::insertNewProfile(string usr_name,string keyP,string name,string ans){
	wallet.open("key.csv",ios::out|ios::app);
	string temp;
	temp=usr_name+","+keyP+","+name+","+ans+"\n";
	if (wallet.is_open()){
		if (keyP.length() < 8){
			cerr<<"password should be atleast 8 characters long !\n";
			exit(0);
		}
		else if (lock.find(usr_name)==lock.end()){
			wallet<<temp;
			loadLock();	
		}
		else
		{
			cerr<<"username already exist!\n";
			exit(0);
		}
		wallet.close();
	}
}

bool biomet::verifyUsr(string usr_name){
	if (lock.empty())
	{
		loadLock();
	}
	if (lock.find(usr_name)==lock.end()){
		return false;
	}
	else return true;
}

bool biomet::verifyPaswd(string name,string	keyP){
	if (lock.empty())
	{
		loadLock();
	}
	if (lock.find(name)==lock.end()){
		cerr<<"username Invalid\n";
		return false;
	}
	else if (lock.at(name)==keyP){
		return true;
	}
	else return false;
}

string  biomet::forgotpassword(string usr_name,string ans){
    if (lock.empty() || Safe.empty())
	{
		loadLock();
	}
	if(verifyUsr(usr_name)){
		if(Safe.at(usr_name)==ans){
			return lock.at(usr_name);
		}
		else{
			//cerr<<"security question's answer not matched\n";
			return "notok";
		}
	}
	else{

		return "notok";	
	}
}



/*int main (){
	biomet Pitara;
	Pitara.loadLock();
	//Pitara.insertNewProfile("cs1130201","hawahawai","aayush gautam0","lucky");
	cout<<Pitara.verifyUsr("aayush")<<endl;
	//string a=Pitara.forgotpassword("cs1130201","lucky");
	//cout <<a;

}*/