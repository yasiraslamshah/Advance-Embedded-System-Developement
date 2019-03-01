//Author:Yasir Aslam Shah
//HW4
//AESD Spring 2019
//client.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <time.h>

#define MAXBUFSIZE 100
#define MAXBUFSIZE1 10000
#define buffsize 1

FILE *fp;
/*MAIN FUNCTION*/
int main (int argc, char * argv[])
{
  struct timespec start,stop,start1,stop1;
  fp = fopen("socket_log.log","a");
  int nbytes,nbytes1;                   // number of bytes send by sendto()
  int sock;                               //this will be our socket
	unsigned int remote_length;
  struct sockaddr_in remote;              //"Internet socket address structure"
	int port =9999;        
  bzero(&remote,sizeof(remote));               //zero the struct
  remote.sin_family = AF_INET;                 //address family
	remote.sin_port = htons(port);      //sets port to network byte order

  struct hostent *host =gethostbyname(argv[1]);
  memcpy(&remote.sin_addr,host->h_addr,host->h_length);
  remote.sin_addr.s_addr = inet_addr(argv[1]); //sets remote IP address
        //Causes the system to create a generic socket of type UDP (datagram)
  printf("\nHI2\n");
 if ((sock = socket(AF_INET, SOCK_STREAM,0)) < 0)
  {
    printf("unable to create socket\n");
    fprintf(fp,"\nError: Socket Creation\n");
  }
	else 
  {
    printf("socket created succesfully\n");
    fprintf(fp,"\nSocket Created\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    fprintf(fp,"\nTimestamp Socket Start %ld\n",(start.tv_sec*(long)1e9)+start.tv_nsec);
  }

  if (connect(sock,(struct sockaddr *) &remote,sizeof(remote)) < 0)
  { 
    printf("ERROR Connection\n");
    fprintf(fp,"\nError: Socket Connection\n");
  }

  else
  {
    printf("\nconnected!\n");
    fprintf(fp,"\nConnected\n");
  }
  
  for(int i=0;i<10;i++)
  {
    char buffer1[50];
    sprintf(buffer1,"Client Says :Mark %d",i);
    fprintf(fp,"\nSending to Parent from Client :%s\n",buffer1);

    nbytes =write(sock,&buffer1,sizeof(buffer1));
    if(nbytes < 0)
    {
      printf("\n\error in sending\n");
      fprintf(fp,"\nError: Sending by Client\n");
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(fp,"\nTimestamp Write %ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);

    char buffer2[50];
    bzero(buffer2,sizeof(buffer2));
    nbytes1 = read(sock,&buffer2,sizeof(buffer2));
    if(nbytes1 < 0)
    {
      printf("Error in reading\n");
    }
    printf("\nString from server: %s Length: %d\n",buffer2,sizeof(buffer2));
    fprintf(fp,"\nString Recieved from Server : %s %d \n",buffer2,sizeof(buffer2));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
    fprintf(fp,"\nTimestamp Read %ld\n",(stop1.tv_sec*(long)1e9)+stop1.tv_nsec);

  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
  fprintf(fp,"\nTimestamp Socket END %ld\n",(stop.tv_sec*(long)1e9)+stop.tv_nsec);

  fprintf(fp,"\nClosing Connection!\n");
  fclose(fp);
  close(sock);
return 0;
}
