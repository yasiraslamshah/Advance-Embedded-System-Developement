/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : socket.h
* Creation Date : 20-03-2019
* Last Modified : Wed 20 March 2019 00:08:56 PM MDT
* Created By : Harsimransingh
* Description: header file to use sockets for IPC communication
* Functions:
*           socket_function() to handle external communication 
* References:
*           https://www.geeksforgeeks.org/socket-programming-cc/
*           https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
*           https://github.com/harsimran2421/AESD-5013-002-/tree/master/Assignment-4/sockets
_._._._._._._._._._._._._._._._._._._._._.*/



#ifndef __SOCKET_H
#define __SOCKET_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#define MAXBUFSIZE 200
#define SEND_BUFFER_SIZE 512
#define RECIEVE_BUFFER_SIZE 512
#define PORT_NUMBER 9000


/**
 * @brief 
 * socket_function() is a function called when socket pthread is created. It connects with other processes using SOCK STREAM(TCP) and provides instantaneous temperature or light values depending on what is requested from the external process.
 *  
 * @param arg
 *
 * @return void since it is a function linked to a pthread
 */
void *socket_function(void *arg);

#endif
