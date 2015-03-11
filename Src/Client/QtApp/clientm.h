
#ifndef CLIENTM_H
#define CLIENTM_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace std;
string main2(char*hostname, char* portnum, char* request,char * userName, char *password, char *shareWith,char *filename, char* filepath,int version,char* secAns);


#endif // CLIENTM_H
