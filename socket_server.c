#include <sys/types.h>
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
#include <string.h>
#include <time.h>
#define LISTENQ 8 //maximum number of client connection
FILE *fptr;
int main ()
{
  struct timespec start,stop,start1,stop1;
  fptr =fopen("socket_log.log","a");
  int sock, con;                           //This will be our socket
  struct sockaddr_in sin,cliaddr;     //"Internet socket address structure"
  unsigned int port=9999;         //length of the sockaddr_in structure 
  socklen_t clilen;
  
  bzero(&sin,sizeof(sin));                    //zero the struct
  sin.sin_family = AF_INET;                   //address family
  sin.sin_port = htons(port);        //htons() sets the port # to network byte order
  sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine
  //Causes the system to create a generic socket of type UDP (datagram)
  if ((sock = socket(AF_INET, SOCK_STREAM,0)) < 0)
  {
    printf("unable to create socket");
  }
  else
  {
    printf("Socket Created Succesfully!\n");
    fprintf(fptr,"\nSocket Created Successfully\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    fprintf(fptr,"\nTime Stamp Socket Start:%ld\n",(start.tv_sec*(long)1e9)+start.tv_nsec);
  }
  if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    printf("unable to bind socket\n");
  }
  else
  {
    printf("Binded Successfully!\n");
    fprintf(fptr,"\nSocket Binded Successfully\n");
  }
    //listen to the socket by creating a connection queue, then wait for clients
  if(listen (sock, LISTENQ)<0)
  {
    printf("\nError in Listening\n");
  }
  else
  {
    printf("\nListning !\n");
    fprintf(fptr,"\nListening!\n");
  }

  clilen -sizeof(cliaddr);
  con = accept(sock, (struct sockaddr *) &cliaddr, &clilen);
  if (con < 0)
  {
    printf("accept failed %d\n",con);
  }
  else
  {
    printf("%s\n","Accepted.\nReceiving ...");
    fprintf(fptr,"\nAccepted!\n");
  }   

  for(int i=0;i<10;i++)
  {
    char buffer1[50];
    bzero(buffer1,sizeof(buffer1));
    int recv_bytes = read(con, buffer1,sizeof(buffer1));
    printf("\nString from Client: %s Length: %d\n",buffer1,strlen(buffer1));
    fprintf(fptr,"\nString from Client:%s,Length:%d\n",buffer1,strlen(buffer1));
    
    if(recv_bytes<0)
    {
	    printf("\nError:READ FAILED!\n");
      fprintf(fptr,"\nRead Failed!\n");
    }
    char buffer3[50];
    sprintf(buffer3,"Server Says :HIT %d",i);
    fprintf(fptr,"\nString to Write from Server:%s\n",buffer3);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(fptr,"\nTime Stamp Read:%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);

    int nbytes = write(con,buffer3, sizeof(buffer3));
    if(nbytes<0)
    {
      printf("\nError: Sending Failed\n");
      fprintf(fptr,"\nSending Failed\n");
    }
    else
    {
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
      fprintf(fptr,"\nTime Stamp Write:%ld\n",(stop1.tv_sec*(long)1e9)+stop1.tv_nsec);

    }
  }
  fprintf(fptr,"\nClosing Server\n");
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
  fprintf(fptr,"\nTime Stamp Socket Close:%ld\n",(stop.tv_sec*(long)1e9)+stop.tv_nsec);

  fclose(fptr);
  close(con);
  close(sock);
  return 0;
}


