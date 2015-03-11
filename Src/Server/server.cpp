#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
//#include <malloc/malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <pthread.h>
#include "key.cpp"
#include "user.cpp"
#define FAIL    -1
 
using namespace std;
int BUF_SIZE=1024;
int large_Buf=102400000;
fstream destination;

int OpenListener(int port)
{   int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
 
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
 
}
SSL_CTX* InitServerCTX(void)
{   const SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = SSLv3_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
 
void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
void share(SSL* ssl,char *username){
    char buf[BUF_SIZE];
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    char buf3[BUF_SIZE];
    //char* filename,*sharewithThisUser,*filepath;
    //char reply[BUF_SIZE];
    //char buf2[BUF_SIZE];
    string thisuser,thefile,thepath,shareuser;
    thisuser.assign(username,username+strlen(username));    
    int version;
    int sd, bytes;
    bzero(buf,BUF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));
    if(bytes<=0){
        //cout<<"Server received nothing in share "<<endl;
    }
    buf[bytes]=0;
    char* filename=buf;
    thefile.assign(filename,filename+strlen(filename));
    bzero(buf1,BUF_SIZE);
    bytes=SSL_read(ssl,buf1,sizeof(buf1));
    if(bytes<=0){
        //cout<<"Server received nothing in share"<<endl;
    }
    buf1[bytes]=0;
    char* filepath=buf1;
    thepath.assign(filepath,filepath+strlen(filepath));
    bzero(buf2,BUF_SIZE);
    bytes=SSL_read(ssl,buf2,sizeof(buf2));
    if(bytes<=0){
       // cout<<"Server received nothing in share"<<endl;
    }
    buf2[bytes]=0;
    char* sharewithThisUser=buf2;
    shareuser.assign(sharewithThisUser,sharewithThisUser+strlen(sharewithThisUser));
    bzero(buf3,BUF_SIZE);
    bytes=SSL_read(ssl,buf3,sizeof(buf3));//received version
    if(bytes<=0){
        //cout<<"Server received nothing in share"<<endl;
    }
    buf3[bytes]=0;
    version=atoi(buf3);
    
    pitara_user aClient(thisuser);
    clientData temp=aClient.makeClientData(thefile,thepath,version);
    aClient.shareFile(temp,shareuser);
    
    //change the user files info database using the above values

    //cout<<"yooo shared it dude"<<endl;
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}

void deleteThisFile(SSL* ssl,char* username){
    char buf[BUF_SIZE],buf2[BUF_SIZE];
    char* filename,*filePath;
    char reply[BUF_SIZE];
    int sd, bytes;
    biomet verification;
    verification.loadLock();
   // cout<<"Server will accept ssl now in loginCheck"<<endl;
    bzero(buf,BUF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));//reading filename
    if(bytes<=0){
       // cout<<"Server received nothing in login check"<<endl;
    }
    buf[bytes]=0;
    filename=buf;
    bzero(buf2,BUF_SIZE);
    bytes=SSL_read(ssl,buf2,sizeof(buf2));//reading filepath
    if(bytes<=0){
        //cout<<"Server received nothing in login check"<<endl;
    }
    buf2[bytes]=0;
    filePath=buf2;
    string thisuser,thefile,thepath;
    thisuser.assign(username,username+strlen(username));
    thefile.assign(filename,filename+strlen(filename));
    thepath.assign(filePath,filePath+strlen(filePath));
    pitara_user thisUser(thisuser);
    clientData temp1;
    temp1=thisUser.makeClientData(thefile,thepath,1);
    thisUser.removeFile(temp1);
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}


void loginCheck(SSL* ssl, char *userName){
    char buf[BUF_SIZE];
    char* passwordGot;
    char reply[BUF_SIZE];
    int sd, bytes;
    biomet verification;
    verification.loadLock();
    //cout<<"Server will accept ssl now in loginCheck"<<endl;
        bzero(buf,BUF_SIZE);
        bytes=SSL_read(ssl,buf,sizeof(buf));
        if(bytes<=0){
           // cout<<"Server received nothing in login check"<<endl;
        }
        buf[bytes]=0;
        passwordGot=buf;
        //cout<<"is it right: "<<verification.verifyPaswd(userName,passwordGot)<<endl;
        //cout<<"userName: "<<userName<<endl;
        if(verification.verifyPaswd(userName,passwordGot)){
            char* state="right";
            //cout<<"passwordGot: "<<passwordGot<<endl;
            //cout<<"Sending state: "<<state<<endl;
            bytes=SSL_write(ssl,state,strlen(state));
            if(bytes<0){
               // cout<<"server problem loginCheck"<<endl;
            }
        }
        else{
            char* state="wrong";
            //cout<<"passwordGot: "<<passwordGot<<endl;
            //cout<<"Sending state: "<<state<<endl;
            bytes=SSL_write(ssl,state,strlen(state));
            if(bytes<0){
                //cout<<"server problem loginCheck"<<endl;
            }
        }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
} 

void upload2Server(SSL* ssl,char* userName) /* Serve the connection -- threadable */
{   char buf[BUF_SIZE];
    char reply[BUF_SIZE];
    char vers[BUF_SIZE];
    char buf4[BUF_SIZE];
    int sd, bytes;
    int version;
    bzero(buf,BUF_SIZE);
    bytes = SSL_read(ssl, buf, sizeof(buf));  /*recieves file name*/
    if(bytes<0){
            //cout<<"server problem upload2server"<<endl;
    }
   // cout<<"a"<<endl;
    //get the UUID first, that is the server name.
    //buf[bytes]=0;
    char* filename=buf;
    bzero(reply,BUF_SIZE);
    bytes = SSL_read(ssl, reply, sizeof(reply));  /*recieves file path*/
    if(bytes<0){
            //cout<<"server problem upload2server"<<endl;
    }
    //cout<<"b"<<endl;
    //get the UUID first, that is the server name.
    //reply[bytes]=0;
    char* filepath=reply;
    bzero(vers,BUF_SIZE);
    bytes = SSL_read(ssl, vers, sizeof(vers));  /*recieves version*/
    if(bytes<0){
           // cout<<"server problem upload2server"<<endl;
    }
    //cout<<"c"<<endl;
    //get the UUID first, that is the server name.
    //vers[bytes]=0;
    version=atoi(vers);
    //cout<<"filename: "<<filename<<" filepath: "<<filepath<<" version: "<<version<<endl;
    //cout<<endl;
    string thisuser,thefile,thepath;
    thisuser.assign(userName,userName+strlen(userName));
    thefile.assign(filename,filename+strlen(filename));
    thepath.assign(filepath,filepath+strlen(filepath));
    pitara_user aClient(thisuser);
    clientData temp=aClient.makeClientData(thefile,thepath,version);
    string thisIsTheName=aClient.AddFile(temp);
    if (bytes > 0){
        destination.open(thisIsTheName,ios::out|ios::binary);
        //cout<<"storing here"<<buf<<endl;
    }
    else
        ERR_print_errors_fp(stderr);
    if(destination.is_open()) {
        long long size;
        //bzero(buf4,BUF_SIZE);
        bytes = SSL_read(ssl, buf4, sizeof(buf4));
        buf4[bytes]=0;
        //cout<<"lets send the file"<<endl;
            if ( bytes > 0 )
            {
                size=atoi(buf4);
                printf("%lld\n",size);
                int sizerec=0;
                cout<<size<<endl;
                bzero(buf4,BUF_SIZE);
                // int temp=20;
                while(sizerec<size){
                    int a = SSL_read(ssl,buf4,sizeof(buf4));
                    //printf("%s",buff);
                    destination.write(buf4,a);
                    sizerec +=a;
                    //cout<<a<<endl;
                    bzero(buf4,BUF_SIZE);
                    if(a==0) break;
                }
                    //if(sizerec>0){
                        
                    //}
            }
            else ERR_print_errors_fp(stderr);
        destination.close(); 
    }
    else{
        //cout<<"fail"<<endl;
    }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}

string forgotPass(SSL* ssl,char* userName){
    char buf[BUF_SIZE];
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    char buf3[BUF_SIZE];
    //char* filename,*sharewithThisUser,*filepath;
    //char reply[BUF_SIZE];
    //char buf2[BUF_SIZE];
    string thisuser,securityAns;
    thisuser.assign(userName,userName+strlen(userName));    
    int sd, bytes;
    bzero(buf,BUF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));
    if(bytes<=0){
       // cout<<"Server received nothing in share "<<endl;
    }
    buf[bytes]=0;
    char* securityAnswer=buf;
    securityAns.assign(securityAnswer,securityAnswer+strlen(securityAnswer));
    biomet forgot;
    string jawab=forgot.forgotpassword(thisuser,securityAns);
    if(jawab=="notok"){
        char* retMess1="notok";
       // cout<<"server will now say: "<<retMess1<<endl;
        bytes=SSL_write(ssl,retMess1,strlen(retMess1));
        if(bytes<=0){
            //cout<<"Server sent nothing in forgotpass "<<endl;
        }
        sd = SSL_get_fd(ssl);       /* get socket connection */
        SSL_free(ssl);         /* release SSL state */
        close(sd);          /* close connection */
    }
    else{
        char* retMess2=new char[jawab.length() + 1];
        strcpy(retMess2, jawab.c_str()); 
       // cout<<"server will now say: "<<retMess2<<endl;
        bytes=SSL_write(ssl,retMess2,strlen(retMess2));
        if(bytes<=0){
            //cout<<"Server sent nothing in forgotpass "<<endl;
        }
        delete [] retMess2;
        sd = SSL_get_fd(ssl);       /* get socket connection */
        SSL_free(ssl);         /* release SSL state */
        close(sd);          /* close connection */
    }
}

void signMeup(SSL* ssl,char* userName){
    //receive the info string
    //append in the key.cv and other database
    char buf[BUF_SIZE];
    char reply[BUF_SIZE];
    char vers[BUF_SIZE];
    char buf4[BUF_SIZE];
    int sd, bytes;
    //use the above username for username
    //receive password
    //don't receive name
    //receive secans
    char *gotpassword,*gotanswer;
    bytes=SSL_read(ssl,buf,sizeof(buf));//recieve password
    if(bytes<=0){
        //cout<<"Server received nothing in login check"<<endl;
        return;
    }
    buf[bytes]=0;
    gotpassword=buf;
    bytes=SSL_read(ssl,reply,sizeof(reply));//receive answer
    if(bytes<=0){
        //cout<<"Server received nothing in login check"<<endl;
        return;
    }
    reply[bytes]=0;
    gotanswer=reply;
    string thisuser,thepass,thesecans;
    thisuser.assign(userName,userName+strlen(userName));
    thepass.assign(gotpassword,gotpassword+strlen(gotpassword));
    thesecans.assign(gotanswer,gotanswer+strlen(gotanswer));

    
   // cout<<"server will tell the username exists or not: "<<endl;
    biomet signhimup;
    signhimup.loadLock();
    if(signhimup.verifyUsr(thisuser)==false){
        signhimup.insertNewProfile(thisuser,thepass,"same",thesecans);
        char* retMess1="ok";
       // cout<<"server will now say: "<<retMess1<<endl;
        bytes=SSL_write(ssl,retMess1,strlen(retMess1));
         /////same usrename error still remains
        sd = SSL_get_fd(ssl);       /* get socket connection */
        SSL_free(ssl);         /* release SSL state */
        close(sd);          /* close connection */
    }
    else{
        char* retMess2="notok";
       // cout<<"server will now say: "<<retMess2<<endl;
        bytes=SSL_write(ssl,retMess2,strlen(retMess2));
         /////same usrename error still remains
        sd = SSL_get_fd(ssl);       /* get socket connection */
        SSL_free(ssl);         /* release SSL state */
        close(sd);          /* close connection */
    }

   
}

void sendFileInfoBack(SSL* ssl,char* userName){
    //ssl_write the string with file infos
    char buf[BUF_SIZE];
    int sd, bytes;
    string thisuser;
    thisuser.assign(userName,userName+strlen(userName));
    pitara_user aClient(thisuser);
    string retVal=aClient.getUserData();//replace the function with string of the file
    //cout<<"calculating retVal: "<<retVal<<endl;
    if(retVal=="")retVal="NoData";
    long int sent_bytes=0;
    long long int toSend;
    char* sendThis=new char[retVal.length()+1];
    copy(retVal.begin(), retVal.end(), sendThis);
    sendThis[retVal.length()]='\0';
    toSend=strlen(sendThis);
    sprintf(buf,"%lld",toSend+1);

    bytes = SSL_write(ssl, buf, strlen(buf));//sending the size of string
    if(bytes<0){
        //cout<<"server couldn't write for clientDownload"<<endl;
        return;
    }    
    bytes=SSL_write(ssl,sendThis,strlen(sendThis));
    if(bytes<0){
       // cout<<"server couldn't write for clientDownload"<<endl;
        return;
    }
    delete[] sendThis;
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}

void downloadFromServer(SSL* ssl,char *userName) /* Serve the connection -- threadable */
{
    char buf[BUF_SIZE];
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    int sd, bytes;
    int version;
    bzero(buf,BUF_SIZE);
    bytes = SSL_read(ssl, buf, sizeof(buf));  /*recieves file name*/
    if(bytes<0){
            //cout<<"server problem upload2server"<<endl;
    }
    //get the UUID first, that is the server name.
    buf[bytes]=0;
    char* filename=buf;
    bzero(buf1,BUF_SIZE);
    bytes = SSL_read(ssl, buf1, sizeof(buf1));  /*recieves file path*/
    if(bytes<0){
            //cout<<"server problem upload2server"<<endl;
    }
    //get the UUID first, that is the server name.
    buf1[bytes]=0;
    char* filepath=buf1;
    //cout<<"received path1 : "<<filepath<<endl;
    string thepath;
    thepath.assign(filepath,filepath+strlen(filepath));
    bzero(buf2,BUF_SIZE);
    bytes = SSL_read(ssl, buf2, sizeof(buf2));  /*recieves version*/
    buf2[bytes]=0;
    if(bytes<0){
            //cout<<"server problem upload2server"<<endl;
    }
    //get the UUID first, that is the server name.
    buf2[bytes]=0;
    version=atoi(buf2);
    //cout<<endl;
    //cout<<"receiving stuff: "<<filename<<" "<<filepath<<" "<<version<<endl;
    string thisuser,thefile;
    thisuser.assign(userName,userName+strlen(userName));
    thefile.assign(filename,filename+strlen(filename));
    
    pitara_user aClient(thisuser);
    clientData temp=aClient.makeClientData(thefile,thepath,version);
    string serverFileName=aClient.downloadFile(temp);


    if (bytes > 0){
        buf[bytes] = 0;
        destination.open(serverFileName,ios::in|ios::binary);
        //cout<<"storing here"<<buf<<endl;
    }
    else ERR_print_errors_fp(stderr);
        

        if(destination.is_open()) {
            long long size;
            destination.seekg(0,ios::end);
            size=destination.tellg();
            printf("hahah %lld",size);
            sprintf(buf,"%lld",size);
            bytes = SSL_write(ssl, buf, strlen(buf));//the loop variable
            if(bytes<0){
                //cout<<"server couldn't write for clientDownload"<<endl;
                return;
            }
            destination.seekg(0,ios::beg);
            bzero(buf,BUF_SIZE);
            long totalData=0;
           // cout<<"size is: "<<size<<endl;
            while (!destination.eof()){//sizesend<size ) {
                long int initialPoint = destination.tellg();

                destination.read(buf,BUF_SIZE);  //reads from file
                long int finalPoint = destination.tellg();
                if(finalPoint>size or finalPoint<0) {finalPoint=size; cout<<"setting final point: "<<finalPoint<<" to size: "<<size<<endl;}
               // cout<<"finalpoint: "<<finalPoint<<endl;
                long int matterRead = finalPoint-initialPoint;
                long int bytesSent = 0;
               // cout<<"matterRead: "<<matterRead<<endl;
                while(bytesSent<matterRead){

                    int b = SSL_write(ssl, buf,matterRead);  //sends the loop var.
                    //buf[b]=  '\0';
                   // cout<<"sent data: "<<b<<endl;
                    if(b<0){
                       // cout<<"file not sent"<<endl;
                        break;
                    }
                    bytesSent += b;
                }
                if(totalData<0)break;
                totalData+=bytesSent;
                bzero(buf,BUF_SIZE); 
            }
           // cout<<"total data sent: "<<totalData<<endl;



            destination.close(); 
        } 
        else
            ERR_print_errors_fp(stderr);
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);
}

void directToRequest(SSL* ssl) /* Serve the connection -- threadable */
{   
    char buf[BUF_SIZE];

    char reply[BUF_SIZE];
    int sd, bytes;
    char *username,*requested;
    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
    else
    {
        //cout<<"Should server showCerts in directtoRequest"<<endl;
        ShowCerts(ssl);        /* get any certificates */
        bzero(buf,BUF_SIZE);
        int readBytes=SSL_read(ssl,buf,sizeof(buf));
        buf[readBytes]=0;
        if(readBytes<0){
            //cout<<"Server could not read"<<endl;
            return;
        }
        //cout<<"Server read: "<<buf<<endl;
        username=buf;
        bzero(reply,BUF_SIZE);
        readBytes=SSL_read(ssl,reply,sizeof(reply));
        reply[readBytes]=0;
        if(readBytes<0){
            //cout<<"Server could not read"<<endl;
            return;
        }
        //cout<<"Server read: "<<reply<<endl;
        requested=reply;
        //cout<<"username changed to: "<<username<<endl;
        if (strcmp(requested,"download")==0){
            downloadFromServer(ssl,username);
        }
        else if (strcmp(requested,"upload")==0){
            upload2Server(ssl,username);
        }
        else if (strcmp(requested,"login")==0){
            //cout<<"server will loginCheck now"<<endl;
            loginCheck(ssl,username);
        }
        else if (strcmp(requested,"showmyFiles")==0){
            sendFileInfoBack(ssl,username);
        }
        else if (strcmp(requested,"deleteFromServer")==0){
            deleteThisFile(ssl,username);
        }
        else if (strcmp(requested,"share")==0){
            share(ssl,username);
        }
        else if (strcmp(requested,"signup")==0){
            signMeup(ssl,username);
        }
        else if (strcmp(requested,"forgotPassword")==0){
            forgotPass(ssl,username);
        }
        else{
            //cout<<"Invalid request received"<<endl;
            sd = SSL_get_fd(ssl);       /* get socket connection */
            SSL_free(ssl);         /* release SSL state */
            close(sd);
        }

    }
    //sd = SSL_get_fd(ssl);       /* get socket connection */
    //SSL_free(ssl);         /* release SSL state */
    //close(sd);
}     


int main(int count, char *strings[])
{   SSL_CTX *ctx;
    int server;
    char *portnum;
    char buf[BUF_SIZE];
    int readBytes;
    char* username;
    char* requested;
    /*
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }*/
    if ( count != 2 )
    {
        printf("Usage: %s <portnum>\n", strings[0]);
        exit(0);
    }
    SSL_library_init();
 
    portnum = strings[1];
    ctx = InitServerCTX();        /* initialize SSL */
    
    LoadCertificates(ctx, "mycert.pem", "mycert.pem"); /* load certs */
    server = OpenListener(atoi(portnum));    /* create server socket */
    while (1)
    {   

        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        SSL *ssl;
 
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_CTX_set_mode(ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);
        SSL_set_mode(ssl, SSL_MODE_ENABLE_PARTIAL_WRITE);
    
        SSL_set_fd(ssl, client);    /* set connection socket to SSL state */  
       
        pid_t child;
        child=fork();

        if (child >= 0){
            if (child == 0){
                   // cout<<"Debug: reached in child process"<<endl;
                    directToRequest(ssl);     /* service connection */        
            }
        }
        else {
            perror("failed\n");
        }
    }
    close(server);          /* close server socket */
    SSL_CTX_free(ctx);         /* release context */
}




