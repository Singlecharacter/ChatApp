#pragma once

#include <string>
#include <vector>

const std::string SERVER_PORT = "55632";
const int BACKLOG_SIZE = 10;
const int MAXDATASIZE = 1024;

const int SERVER_CHAT_CHANNEL = 0;
const int SERVER_CHAT_USER = 1;
const int SERVER_GENERAL = 2;
const int SERVER_ACCEPT = 3;

const int USER_CHAT_CHANNEL = 10;
const int USER_CHAT_USER = 11;
const int USER_CONNECT_CHANNEL = 12;
const int USER_DISCONNECT_CHANNEL = 13;
const int USER_SET_NAME = 14;
const int USER_CREATE_CHANNEL = 15;


struct user
{
    std::string name;
    int sock;
};

struct channel
{
    std::vector<user*> members;
    std::string name;

    channel* next;
};

