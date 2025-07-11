#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<unistd.h>
#include<poll.h>
#include<signal.h>
#define MAX_CLIENTS 10
#define MAX_NAME_SIZE 50
struct pollfd client[10];
char** create2dim(int rows, int columns);
void del2dim(char** arr, int rows);
void signalpipe(int signum);
int main()
{   
    signal(SIGPIPE,signalpipe);
    char **checknames=create2dim(MAX_CLIENTS,MAX_NAME_SIZE);
    int checkfdnum[MAX_CLIENTS];

    for(int i=0;i<MAX_CLIENTS;i++)
    {   
        client[i].events=POLLIN|POLLHUP;
        client[i].fd=-1;
    }
    char *buffer=malloc(4096);

    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("cant open socket\n");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serveraddress;
    memset(&serveraddress,0,sizeof(serveraddress));
    serveraddress.sin_family=AF_INET;
    int networkport=htons(7777);
    inet_pton(AF_INET,"127.0.0.1",&serveraddress.sin_addr.s_addr);
    serveraddress.sin_port=networkport;
    int sizeofserveraddress=sizeof(serveraddress);
    int checkbind=bind(sockfd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    if(listen(sockfd,MAX_CLIENTS)==-1)
    {
        perror("error:doesn't work listen\n");
        exit(1);
    }
    struct pollfd poll_con_socket;
    poll_con_socket.fd=sockfd;
    poll_con_socket.events=POLLIN;
    while(1)
   {
      poll(&poll_con_socket,1,100);
     if(poll_con_socket.revents & POLLIN)
     {
        
        for(int i=0;i<MAX_CLIENTS;i++)
        {
          if(client[i].fd==-1)
          {
            client[i].fd=accept(sockfd,(struct sockaddr*)&serveraddress,&sizeofserveraddress);
            printf("there is client\n");
            int readnamebytes=read(client[i].fd,checknames[i],MAX_NAME_SIZE);
            checknames[i][readnamebytes]='\0';
            printf("%d\n",client[i].fd);
            printf("%s",checknames[i]);
            break;
          }
        }
     }
     int count_readable_socket=poll(client,10,3000);
     printf("poll done-%d",count_readable_socket);
     if(count_readable_socket>0)
     {
        printf("there is something to read\n");
       for(int i=0;i<MAX_CLIENTS;i++)
       {
        if(client[i].revents&POLLHUP)
        {
            printf("pollhup is happened\n");
            close(client[i].fd);
            client[i].fd=-1;
            memset(checknames[i],0,50);
        }
         if(client[i].revents&POLLIN)
         {
            printf("there is something to read and send\n");
            int checkmessagebytes=read(client[i].fd,buffer,4095);
            printf("%d-bytes read\n",checkmessagebytes);
            buffer[checkmessagebytes]='\0';
            fputs(buffer,stdout);  //print in server to check
            char *delimeter=strchr(buffer,62);
            
            for(int j=0;j<MAX_CLIENTS;j++)
            {
                if(delimeter==NULL)
                {
                  if(client[j].fd>0)
                  {
                    write(client[j].fd,buffer,strlen(buffer));
                  }
                }
                else
                {
                    *delimeter='\0';
                  if(!strcmp(buffer,checknames[j]))
                  {
                    write(client[j].fd,delimeter+1,strlen(delimeter+1));
                  }
                }
                
            }
         }
        }
     }
     printf("new cycle\n");
        
    }
    
   
}
char** create2dim(int rows, int columns) 
{
    char** arr2dim=(char**)malloc(rows*sizeof(char*));
    if(arr2dim==NULL)
    {
        perror("dynamic memory for buildin list is failed");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<rows;i++)
    {
        arr2dim[i]=malloc(columns*(sizeof(char)));
        if(arr2dim[i]==NULL)
        {
            perror("dynamic memory for buildin list is failed");
            exit(EXIT_FAILURE); 
        }
    }
    return arr2dim;
}
void del2dim(char** arr, int rows)
{
    for(int i=0;i<rows;i++)
    {
        free(arr[i]);
    }
    free(arr);
}
void signalpipe(int signum)
{
    printf("SIGPIPE\n");
   // exit(1);
}