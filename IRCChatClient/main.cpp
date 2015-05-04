// ConsoleApplication2.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <pthread.h>

#include "Message.h"
#include "MessageBuilder.h"
#include "defines.h"

#include <arpa/inet.h>

#define PORT "55632" // the port client will be connecting to

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *recv_loop(void *a)
{
    int sockfd = *((int *)a);

    while(1)
    {
        char buf[MAXDATASIZE];

        int numbytes = recv(sockfd,buf,MAXDATASIZE - 1,0);
        if(numbytes > 0)
        {
            buf[numbytes] = '\0';
            std::string bufString = buf;

            Message newMessage(bufString);

            int messageType = newMessage.getInt("type");

            if(messageType == SERVER_ACCEPT)
            {
                std::cout << "Connected to server." << std::endl;
            }
            else if(messageType == SERVER_GENERAL)
            {
                std::cout << newMessage.getString("main") << std::endl;
            }
            else if(messageType == SERVER_CHAT_CHANNEL)
            {
                std::cout << "<" + newMessage.getString("channel") + ">" + newMessage.getString("name") + ": " + newMessage.getString("main") << std::endl;
            }
        }
    }

    return nullptr;
}

void *input_loop(void *a)
{
    int sockfd = *((int *)a);

    std::string s;

    std::string currentChannel = "default";

    while(true)
    {
        std::getline(std::cin, s);

        std::string command = "";

        int commandType = USER_CHAT_CHANNEL;

        //Check for special commands
        if(s[0] == '#')
        {
            command = s.substr(1, s.find(' ', 0) - 1);

            if(command == "name")
            {
                commandType = USER_SET_NAME;
            }
            else if(command == "channel")
            {
                commandType = USER_CONNECT_CHANNEL;
            }
            else if(command == "createchannel")
            {
                commandType = USER_CREATE_CHANNEL;
            }
        }

        if(command != "")
        {
            s.erase(0, s.find(' ', 0)+1);
        }

        Message sendMessage;

        if(commandType == USER_CHAT_CHANNEL)
        {
            sendMessage = (new MessageBuilder())->putInt("type",commandType)->putString("main",s)->putString("channel",currentChannel)->build();
        }
        else if(commandType == USER_SET_NAME)
        {
            sendMessage = (new MessageBuilder())->putInt("type",commandType)->putString("name",s)->build();
        }
        else if(commandType == USER_CONNECT_CHANNEL)
        {
            sendMessage = (new MessageBuilder())->putInt("type",commandType)->putString("channel",s)->build();
        }
        else if(commandType == USER_CREATE_CHANNEL)
        {
            sendMessage = (new MessageBuilder())->putInt("type",commandType)->putString("channel",s)->build();
        }

        send(sockfd, sendMessage.getRaw().c_str(),sendMessage.getRaw().length(),0);
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    pthread_t serv_thread, input_thread;

    pthread_create(&serv_thread,NULL,recv_loop,&sockfd);
    pthread_create(&input_thread,NULL,input_loop,&sockfd);

    pthread_join(serv_thread,NULL);

    close(sockfd);

    return 0;
}
