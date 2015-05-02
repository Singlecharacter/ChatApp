#pragma once

#include <string>

const std::string SERVER_PORT = "55632";
const int BACKLOG_SIZE = 10;
const int MAXDATASIZE = 999;

const int SERVER_CHAT_CHANNEL = 0;
const int SERVER_CHAT_USER = 1;
const int SERVER_GENERAL = 2;

const int USER_CHAT_CHANNEL = 10;
const int USER_CHAT_USER = 11;
const int USER_CONNECT_CHANNEL = 12;
const int USER_DISCONNECT_CHANNEL = 13;
