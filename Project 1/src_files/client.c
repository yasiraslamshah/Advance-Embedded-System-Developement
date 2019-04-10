#include "socket.h"
#include "logger.h"

void main()
{
  int client_id = 0;
  int nbytes_send = 0;
  int nbytes_recieve = 0;
  struct sockaddr_in server_address;
  char recieve_command[10];
  client_id = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT_NUMBER);
  inet_pton(AF_INET,"127.0.0.1",&server_address.sin_addr);
  int connection_status = connect(client_id, (struct sockaddr *) &server_address, sizeof(server_address));
  printf("Successfully connected to server!!!\n");
  printf("\nCOMMAND OPTIONS\n1. TEMPC\n2. TEMPK\n3. TEMPF\n4. LIGHT\n\n");
  printf("Enter command:");
  gets(recieve_command);
  printf("RECIEVED COMMAND IS:%s\n",recieve_command);
  if(strcmp(recieve_command,"TEMPC") == 0 || strcmp(recieve_command,"TEMPK") == 0 || strcmp(recieve_command,"TEMPF") == 0 || strcmp(recieve_command,"LIGHT") == 0)
  {
    nbytes_send = send(client_id,(char *)recieve_command,strlen(recieve_command),0);
    if(nbytes_send < 0)
    {
      printf("error sending\n");
      return;
    }
    else
    {
      printf("\nRECIEVING COMMAND\n");
      msg_struct msg;
      nbytes_recieve = recv(client_id,(char *)&msg,sizeof(msg),0);
      if(nbytes_recieve < 0)
      {
        printf("\nERROR RECIEVING\n");
      }
      else
      {
        printf("\nREACHED HERE %s\n",msg.level);
        if(strncmp(msg.level,"DATA",4) == 0)
        {
          printf("\n\n%s\n\n",msg.level);
          printf("SENSOR:%s\nSensor value: %0.2f%c\n",msg.thread_name,msg.sensor_value,msg.unit);
        }
        else
        { 
          printf("\nSENSOR ERROR\n");
        }
      }
    }
  }
  else
  {
    printf("invalid command!!\n");
    return;
  }
}
