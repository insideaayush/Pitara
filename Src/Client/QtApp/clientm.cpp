#include "clientm.h"
#define FAIL    -1
#include <QMessageBox>
int BUFFF_SIZE=1024;
using namespace std;

fstream source;
int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd=socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

SSL_CTX* InitCTX(void)
{   const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = SSLv3_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
        cout<<"i am null"<<endl;
    }
    return ctx;
}

void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("No certificates.\n");
}

string login(SSL* ssl,char* password){
    char buf[BUFFF_SIZE];
    int bytes;

    bytes=SSL_write(ssl,password,strlen(password));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        return "";
    }
    bzero(buf,BUFFF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        return "";
    }
    char *state;
    state=buf;
    //cout<<"received state: "<<state<<endl;
    if(strcmp(state,"right")==0){
        //then login
        //cout<<"Your username password is correct"<<endl;
        SSL_free(ssl);
        return "ok";
    }
    else{
        //do not login
        //cout<<"Your username password is incorrect"<<endl;
        SSL_free(ssl);
        return "notok";
    }
}

string signUp(SSL* ssl,char* clientpassword,char* clientsecans){

   // name="bc";
    //clientUsername="bc";
    //clientPassword="hi!baby!Sexy!";
    //clientSecAns="sarangi";
    //send these values
    char buf[BUFFF_SIZE];
    int bytes;
    /*We have already sent the user name*/
    //bytes=SSL_write(ssl,userName,strlen(userName));
    //if(bytes<=0){
    //    cout<<"Couldn't connect to server"<<endl;
    //    return;
    //}
    //cout<<"a"<<endl;
    bytes=SSL_write(ssl,clientpassword,strlen(clientpassword));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    //cout<<"a"<<endl;

    bytes=SSL_write(ssl,clientsecans,strlen(clientsecans));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    //cout<<"a"<<endl;
    bzero(buf,BUFFF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));//getthelastmessage
    if(bytes<0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    buf[bytes]=0;
    //cout<<"a"<<endl;
    char* state1=buf;
    char* state2="notok";
   // cout<<state1;
    SSL_free(ssl);
    if(strcmp(state1,"ok")==0){
        return "ok";
    }
    else{
        return "notok";
    }
    //char* send=name+client
}

string showmyFiles(SSL* ssl){
    //ssl_read the string that has all the info
    char buf[BUFFF_SIZE];
    int bytes; 
    int toRead;
    bzero(buf,BUFFF_SIZE);
    toRead=SSL_read(ssl,buf,sizeof(buf));//receiving the size of string
    if(toRead<0){
       // cout<<"client couldn't read for clientFileInfo"<<endl;
        SSL_free(ssl);
        return "";
    }
    toRead=atoi(buf);
    char receiveThis[toRead];
    bytes=SSL_read(ssl,receiveThis,sizeof(receiveThis));
    if(bytes<0){
        //cout<<"client couldn't read for clientFileInfo"<<endl;
        SSL_free(ssl);
        return "";
    }
    //process it and display on the table
    string theinformation;
    theinformation.assign(receiveThis,receiveThis+strlen(receiveThis));
    SSL_free(ssl);
    //cout<<theinformation<<endl;
    return theinformation;
}

string share(SSL* ssl, char* fileName,char* filepath, char* shareWith, int version){
    char buf[BUFFF_SIZE];
    int bytes;
    /*We have already sent the user name*/
    //bytes=SSL_write(ssl,userName,strlen(userName));
    //if(bytes<=0){
    //    cout<<"Couldn't connect to server"<<endl;
    //    return;
    //}
    bytes=SSL_write(ssl,fileName,strlen(fileName));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    bytes=SSL_write(ssl,filepath,strlen(filepath));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    bytes=SSL_write(ssl,shareWith,strlen(shareWith));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }

    bzero(buf,BUFFF_SIZE);
    sprintf(buf,"%d",version);
    bytes=SSL_write(ssl,buf,sizeof(buf));/*send the version of the file*/
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    SSL_free(ssl);
    return "1";
}

string forPass(SSL* ssl, char* ansOfSec){
    char buf[BUFFF_SIZE];
    int bytes;
    /*We have already sent the user name*/
    //bytes=SSL_write(ssl,userName,strlen(userName));
    //if(bytes<=0){
    //    cout<<"Couldn't connect to server"<<endl;
    //    return;
    //}
    bytes=SSL_write(ssl,ansOfSec,strlen(ansOfSec));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }

    bzero(buf,BUFFF_SIZE);
    bytes=SSL_read(ssl,buf,sizeof(buf));
    if(bytes<0){
       // cout<<"client couldn't read for clientFileInfo"<<endl;
        SSL_free(ssl);
        return "";
    }
    char* state=buf;
    SSL_free(ssl);
    if(strcmp(state,"notok")==0){
        return "notok";
    }
    else{
        string retValue;
        retValue.assign(state,state+strlen(state));
        return retValue;
    }
    return "1";
}


string deleteFromServer(SSL* ssl,char* fileName,char* filepath){
    char buf[BUFFF_SIZE];
    int bytes;
    bytes=SSL_write(ssl,fileName,strlen(fileName));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    bytes=SSL_write(ssl,filepath,strlen(filepath));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    return "1";
}

string upload2server(SSL* ssl,char* filename, char* filepath,int version){
    source.open(filepath,ios::in|ios::binary);
    //cout<<"filename: "<<filename<<"filepath: "<<filepath<<"itsversion: "<<version<<endl;
    //if(source.is_open()){
    int bytes_sent=SSL_write(ssl, filename, strlen(filename));
    //^sends file name

    char buf[BUFFF_SIZE];
    //bzero(buf,sizeof(buf));
    bytes_sent=SSL_write(ssl,filepath,strlen(filepath));
    if(bytes_sent<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    bzero(buf,BUFFF_SIZE);
    sprintf(buf,"%d",version);
    //cout<<"sending version: "<<buf<<endl;
    bytes_sent=SSL_write(ssl,buf,sizeof(buf));/*send the version of the file*/
    if(bytes_sent<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    if (source.is_open()) {
        bzero(buf,BUFFF_SIZE);
        source.seekg(0,ios::end);
        long int size=source.tellg();   //calculates loop variable to be sent
        sprintf(buf,"%ld",size);

        source.seekg (0, ios::beg);
        bytes_sent=SSL_write(ssl, buf, strlen(buf));  //sends the loop var.
        //buf[bytes_sent]='\0';
        //int sizesend=0;
        bzero(buf,BUFFF_SIZE);
        //int temp=20;
        long totalData=0;
        //cout<<"size is: "<<size<<endl;
        while (!source.eof()){//sizesend<size ) {
            long int initialPoint = source.tellg();

            source.read(buf,BUFFF_SIZE);  //reads from file
            long int finalPoint = source.tellg();
            if(finalPoint>size or finalPoint<0) {finalPoint=size;}// cout<<"setting final point: "<<finalPoint<<" to size: "<<size<<endl;}
            //cout<<"finalpoint: "<<finalPoint<<endl;
            long int matterRead = finalPoint-initialPoint;
            long int bytesSent = 0;
            //cout<<"matterRead: "<<matterRead<<endl;
            while(bytesSent<matterRead){

                int b = SSL_write(ssl, buf,matterRead);  //sends the loop var.
                //buf[b]=  '\0';
                //cout<<"sent data: "<<b<<endl;
                if(b<0){
                    //cout<<"file not sent"<<endl;
                    break;
                }
                bytesSent += b;
            }
            if(totalData<0) break;
            totalData+=bytesSent;
            bzero(buf,BUFFF_SIZE);
        }
        source.close();
        SSL_free(ssl);
        if(totalData<0)return "";
        else return "1";
        //cout<<"total data sent: "<<totalData<<endl;
        }
        else {
            printf("fail ho gya\n");
            source.close();
            SSL_free(ssl);
            return "";
        }
}


string download(SSL* ssl, char* filename,char* filePath,int version){
    //char buf[BUFF_SIZE];
    char buf[BUFFF_SIZE];
    //sprintf(filePath,"./clientFolder/%s",filename);

    int bytes=SSL_write(ssl, filename, strlen(filename));
    if(bytes<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    //bzero(buf,BUFFF_SIZE);
   //cout<<"sending: "<< filePath<<endl;
    int bytes_sent=SSL_write(ssl, filePath, strlen(filePath));
    if(bytes_sent<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    bzero(buf,BUFFF_SIZE);
    sprintf(buf,"%d",version);
    bytes_sent=SSL_write(ssl,buf,sizeof(buf));/*send the version of the file*/
    if(bytes_sent<=0){
        //cout<<"Couldn't connect to server"<<endl;
        SSL_free(ssl);
        return "";
    }
    //sends file name
    char* partPath="./clientfolder/MyDownloads/";
    char* savePath=new char[strlen(filename)+strlen(partPath)];
    strcpy(savePath,partPath);
    strcat(savePath,filename);
    source.open(savePath,ios::out|ios::binary);
    bzero(buf,BUFFF_SIZE);
    if (source.is_open()){
        bzero(buf,BUFFF_SIZE);
        long int size;   //calculates loop variable to be sent
        bytes_sent=SSL_read(ssl, buf, sizeof(buf));  //receives the loop var.
        //buf[bytes_sent]='\0';
        //int sizesend=0;
        //bzero(buf,BUFF_SIZE);
        if (bytes_sent >0){
            size=atoi(buf);
            printf("%ld\n",size);
            int sizerec=0;
            bzero(buf,BUFFF_SIZE);
            while(sizerec<size){
                int a = SSL_read(ssl,buf,sizeof(buf));
                //printf("%s",buff);
                source.write(buf,a);
                sizerec +=a;
                //cout<<a<<endl;
                bzero(buf,BUFFF_SIZE);
                if(a==0) break;
          }
            source.close();
            delete[] savePath;
            SSL_free(ssl);
            return "1";
        }
        else{
            source.close();
            SSL_free(ssl);
            delete[] savePath;
            return "";
            //ERR_print_errors_fp(stderr);
        }
        //int temp=20;
        }
   else {
        printf("fail ho gya\n");
        delete[] savePath;
        source.close();
        SSL_free(ssl);
        return "";
   }
}


string main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filePath, int version,char* secAns){
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    int sentBytes;
    //char *hostname, *portnum, *request;
    //if ( argc != 4 )
    //{
    //    printf("usage: %s <hostname> <portnum> <request>\n", argv[0]);
    //    exit(0);
    //}
    SSL_library_init();
    //hostname=argv[1];
    //portnum=argv[2];
    //request=argv[3];
    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    SSL_CTX_set_mode(ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);
    SSL_set_mode(ssl, SSL_MODE_ENABLE_PARTIAL_WRITE);

    if ( SSL_connect(ssl) == FAIL ) {  /* perform the connection */
        //QMessageBox::information("Server problem","Couldn't connect to server");
       // cout<<"Couldn't connect to server"<<endl;
        return "";
    }
    else
    {
        //sleep(3);
        sentBytes=SSL_write(ssl,userName,strlen(userName));
        if(sentBytes<=0){
            //cout<<"Failed to connect to server."<<endl;
            return "";
        }
        sentBytes=SSL_write(ssl,request,strlen(request));
        if(sentBytes<=0){
            //cout<<"Failed to connect to server."<<endl;
            return "";
        }
        if(strcmp(request,"download")==0){
                //cout<<"downloading"<<endl;
                string retVal=download(ssl,filename,filePath,version);//
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */
                //cout<<"reached the end"<<endl;
                return retVal;
        }
        else if(strcmp(request,"upload")==0){
                //cout<<"uploading"<<endl;
                string retVal=upload2server(ssl,filename,filePath,version);//,filePath,versions);//
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */
                //cout<<"reached the end"<<endl;
                return retVal;
        }
        else if(strcmp(request,"login")==0){
                //cout<<"logging in"<<endl;
                string answer=login(ssl,password);//
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return answer;//if answer is ok then login else error
        }
        else if(strcmp(request,"showmyFiles")==0){
                //cout<<"i want my files"<<endl;
                string allinfo=showmyFiles(ssl);
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return allinfo;
        }
        else if(strcmp(request,"deleteFromServer")==0){
                //cout<<"delete the file from server"<<endl;
                string retVal=deleteFromServer(ssl,filename,filePath);//
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return retVal;
        }
        else if(strcmp(request,"share")==0){
                //cout<<"share this file"<<endl;
                share(ssl,filename,filePath,shareWith,version);//
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return "1";
        }
        else if(strcmp(request,"signup")==0){
               // cout<<"sign me up dude"<<endl;
                string retVal=signUp(ssl,password,secAns);
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return retVal;
        }
        else if(strcmp(request,"forgotPassword")==0){
               // cout<<"sign me up dude"<<endl;
                string retVal=forPass(ssl,secAns);
                close(server);         /* close socket */
                SSL_CTX_free(ctx);        /* release context */

                //cout<<"reached the end"<<endl;
                return retVal;
        }
        else{
            //cout<<"Invalid Request!"<<endl;
            close(server);         /* close socket */
            SSL_CTX_free(ctx);        /* release context */
            //cout<<"reached the end"<<endl;
            return "";
        }

    }

}
