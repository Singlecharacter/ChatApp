#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "defines.h"
#include "Message.h"
#include "MessageBuilder.h"
#include "stdafx.h"
#include "channellist.h"

//Global declarations
ChannelList channels;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct accept_struct {

    int sockfd;
    sockaddr_storage their_addr;
};

void *client_thread(void *a)
{
    int client_fd = *((int *)a);
    channel *clientChannel = nullptr;

    user *myUser = nullptr;

    while(1)
    {
        char buf[MAXDATASIZE];

        int numbytes = recv(client_fd, buf, MAXDATASIZE - 1, 0);

        if(numbytes > 0)
        {
            buf[numbytes] = '\0';
            std::string bufString = buf;
            std::string replyBody = "";

            Message newMessage(bufString);

            std::cout << "Received message: " << bufString << std::endl;

            int messageType = newMessage.getInt("type");

            if(messageType == USER_SET_NAME)
            {
                std::string newName = newMessage.getString("name");

                if(newName != "")
                {
                    if(myUser == nullptr)
                    {
                        myUser = new user();
                        myUser->name = newName;
                        myUser->sock = client_fd;
                    }
                    else
                    {
                        myUser->name = newName;
                    }

                    replyBody = "Name set as " + newName;
                }
                else
                {
                    replyBody = "No name entered.";
                }
            }
            else if(messageType == USER_CHAT_CHANNEL)
            {
                if(clientChannel != nullptr)
                {
                    Message chatMessage = (new MessageBuilder())->putInt("type",SERVER_CHAT_CHANNEL)
                                                                ->putString("main",newMessage.getString("main"))
                                                                ->putString("channel",clientChannel->name)
                                                                ->putString("name",myUser->name)
                                                                ->build();


                    for(int i = 0; i < clientChannel->members.size(); i++)
                    {
                        send(clientChannel->members.at(i)->sock, chatMessage.getRaw().c_str(), chatMessage.getRaw().length(),0);
                    }

                }
                else
                {
                    replyBody = "You must join a channel first. Use #channel.";
                }
            }
            else if(messageType == USER_CONNECT_CHANNEL)
            {
                if(channels.findChannel(newMessage.getString("channel")) != nullptr)
                {
                    if(myUser != nullptr)
                    {
                        channels.addUser(myUser,newMessage.getString("channel"));
                        replyBody = "Joined channel " + newMessage.getString("channel");

                        clientChannel = channels.findChannel(newMessage.getString("channel"));
                    }
                    else
                    {
                        replyBody = "You must pick a name first. Use #name.";
                    }
                }
                else
                {
                    replyBody = "That channel doesn't exist. You can create it with #createchannel.";
                }
            }
            else if(messageType == USER_CREATE_CHANNEL)
            {
                std::string channelName = newMessage.getString("channel");

                if(channelName != "")
                {
                    channels.addChannel(channelName);
                    replyBody = "Created channel " + channelName;
                }
                else
                {
                    replyBody = "You must enter a channel name.";
                }
            }

            if(replyBody != "")
            {
                Message reply = (new MessageBuilder())->putInt("type",SERVER_GENERAL)->putString("main",replyBody)->build();

                send(client_fd,reply.getRaw().c_str(),reply.getRaw().length(),0);

                std::cout << "Sent reply: " + reply.getRaw() << std::endl;
            }
        }
    }

    return nullptr;
}

void *accept_loop(void *a)
{
    accept_struct as = *((accept_struct *)a);
    int new_fd;

    char s[INET6_ADDRSTRLEN];

    Message acceptMessage = (new MessageBuilder())->putInt("type",SERVER_ACCEPT)->build();
    int acceptLength = acceptMessage.getRaw().length();

    while(1)
    {  // main accept() loop
        socklen_t sin_size = sizeof as.their_addr;
        new_fd = accept(as.sockfd, (struct sockaddr *)&(as.their_addr), &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(as.their_addr.ss_family,
            get_in_addr((struct sockaddr *)&(as.their_addr)),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        pthread_t newClient;

        if (send(new_fd, acceptMessage.getRaw().c_str(), acceptLength, 0) == -1)
            perror("send");

        pthread_create(&newClient,NULL,client_thread, &new_fd);
    }
    return nullptr;
}

int main(void)
{
    int sockfd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, SERVER_PORT.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG_SIZE) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    accept_struct as;
    as.sockfd = sockfd;
    as.their_addr = their_addr;

    pthread_t accept_thread;
    pthread_create(&accept_thread,NULL,accept_loop,&as);

    pthread_join(accept_thread, NULL);

    return 0;
}
