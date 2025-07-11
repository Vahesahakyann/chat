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
int main()
{   int state=1;
    struct pollfd pollclient;
    pollclient.fd=socket(AF_INET,SOCK_STREAM,0);
    if(pollclient.fd==-1)
    {
        perror("cant open socket\n");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serveraddress;
    memset(&serveraddress,0,sizeof(serveraddress));
    serveraddress.sin_family=AF_INET;
    int networkport=htons(7777);
    int serveripaddress;
    inet_pton(AF_INET,"127.0.0.1",&serveripaddress);
    serveraddress.sin_addr.s_addr=serveripaddress;
    serveraddress.sin_port=networkport;
    int checkconnect=connect(pollclient.fd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    
    if(checkconnect==-1)
    {
        perror("connection doesnt happen\n");
    }
    char *buffer=malloc(4096);
    if(buffer==NULL)
    {
        perror("error: failed malloc for buffer\n");
    }
   
    
    fputs("write your username\n",stdout);
    scanf("%s",buffer);
    write(pollclient.fd,buffer,strlen(buffer));
    pollclient.events=POLLOUT|POLLIN|POLLHUP;
   
    while(state)
    {    
        poll(&pollclient,1,2000);
        if(pollclient.revents & POLLIN)
        {  printf("there is something to read!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            int readfromothers=read(pollclient.fd,buffer,4095);
            printf("%d------is read from other clients\n");
            if(readfromothers>0)
            {
                buffer[readfromothers]='\0';
            }
            fputs(buffer,stdout);
        }
        if(pollclient.revents&POLLOUT)
        {
         fputs("write the username and message with this struture: username>[your message]\n",stdout);
         fgets(buffer,4095,stdin);
         if(strlen(buffer)>2)
         {
            int written=write(pollclient.fd,buffer,strlen(buffer));
            printf("%d-bytes written\n",written);
         }
        }
        if(pollclient.revents&POLLHUP)
        {   
            perror("server is unavaible to take action\n");
           
            free(buffer);
            exit(1);
        }

    }
    
}