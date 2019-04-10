#include "logger.h"
#include "light.h"
#include "temperature.h"
#include "socket.h"

void *socket_function(void *arg)
{
  struct sockaddr_in client_address;
  int listen_fd = 0;
  int result_client;
  int addr_length;
  struct sockaddr_in server;
  int nbytes_recieve, nbytes_send;
  char recieve_buffer[MAXBUFSIZE];
  bzero((char *)&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT_NUMBER);

  if((listen_fd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
  {
    printf("ERROR: CREATING SOCKET\n");
    exit(1);
  }

  /*check if bind is success*/
  if((bind(listen_fd,(struct sockaddr *)&server,sizeof(server))) < 0)
  {
    printf("ERROR : BINDING\n");
    exit(1);
  }

  /*listen for incoming connections*/
  if(listen(listen_fd,1000000) != 0)
  {
    perror("ERROR: listen()");
    exit(1);
  }

  addr_length = sizeof(client_address);
  int i = 0;
  int child_pid = 0;
  while(exit_flag != 1)
  {
    printf("\nWaiting To Recieve Command\n");
    result_client = accept(listen_fd,(struct sockaddr*)&client_address , &addr_length);
    if(result_client < 0)
    {
      printf("\nError in accept\n");
      exit(1);
    }
    nbytes_recieve = read(result_client,recieve_buffer,MAXBUFSIZE);
    if(nbytes_recieve < 0)
    {
      printf("\nError while recieving command\n");
      shutdown(result_client, SHUT_RDWR);
      close(result_client);
      //return;
    }
    else if(nbytes_recieve == 0)
    {
      printf("\nNo command recieved\n");
      shutdown(result_client, SHUT_RDWR);
      close(result_client);
      //return;
    }
    else
    { 
      printf("\n\n\nRECIEVED COMMAND IS:%s\n\n\n",recieve_buffer);
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      if(strncmp(recieve_buffer,"TEMPC",5) == 0)
      {
       float temperature_value;
       int result = temp_main(&temperature_value,1); 
       if(result == EXIT_FAILURE)
       {
         printf("\nError socket tempc sensor\n");
         memcpy(msg->level,"ERROR",strlen("ERROR"));
         nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
       }
       else
       {
         memset(msg->thread_name,'\0',sizeof(msg->thread_name));
         memcpy(msg->thread_name,"temp",strlen("temp"));
         msg->sensor_value = temperature_value;
         memset(msg->level,'\0',sizeof(msg->level));
         memcpy(msg->level,"DATA",strlen("DATA"));
         msg->unit = 'C';
         printf("\nSENDING DATA\n");
         nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
         if(nbytes_send < 0)
         {
           printf("\n\nERRO SENDING SENSOR VALUE\n\n");
         }
       }
      }
      else if(strncmp(recieve_buffer,"TEMPK",5) == 0)
      {
        float temperature_value;
        int result = temp_main(&temperature_value,2); 
        if(result == EXIT_FAILURE)
        {
          printf("\nError socket tempk sensor\n");
          memcpy(msg->level,"ERROR",strlen("ERROR"));
          nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
        }
        else
        {
          memset(msg->thread_name,'\0',sizeof(msg->thread_name));
          memcpy(msg->thread_name,"temp",strlen("temp"));
          msg->sensor_value = temperature_value;
          memset(msg->level,'\0',sizeof(msg->level));
          memcpy(msg->level,"DATA",strlen("DATA"));
          msg->unit = 'K';
          nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
        }

      }
      else if(strncmp(recieve_buffer,"TEMPF",5) == 0)
      {
        float temperature_value;
        int result=temp_main(&temperature_value,3); 
        if(result == EXIT_FAILURE)
        {
          printf("\nError socket tempf sensor\n");
          memcpy(msg->level,"ERROR",strlen("ERROR"));
          nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
        }
        else
        {
          memset(msg->thread_name,'\0',sizeof(msg->thread_name));
         memcpy(msg->thread_name,"temp",strlen("temp"));
         msg->sensor_value = temperature_value;
         memset(msg->level,'\0',sizeof(msg->level));
         memcpy(msg->level,"DATA",strlen("DATA"));
         msg->unit = 'F';
         nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
       }

      }
      else if(strncmp(recieve_buffer,"LIGHT",5) == 0)
      {
       float light_value;
       int result = Light_main(&light_value); 
       if(result == EXIT_FAILURE)
       {
         printf("\nError socket light sensor\n");
         memcpy(msg->level,"ERROR",strlen("ERROR"));
         nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
       }
       else
       {
         memset(msg->thread_name,'\0',sizeof(msg->thread_name));
         memcpy(msg->thread_name,"light",strlen("light"));
         msg->sensor_value = light_value;
         memset(msg->level,'\0',sizeof(msg->level));
         memcpy(msg->level,"DATA",strlen("DATA"));
         msg->unit = 'L';
         nbytes_send = send(result_client, (void *)msg, sizeof(*msg), 0);
       }
      }
      free(msg);
    }
  }
}


