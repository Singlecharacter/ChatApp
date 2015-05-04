#ifndef CHANNELLIST_H
#define CHANNELLIST_H

#include "stdafx.h"
#include "defines.h"

class ChannelList
{
    public:
        ChannelList();
        ~ChannelList();

        bool addChannel(std::string name);
        bool removeChannel(std::string name);

        channel *findChannel(std::string name);

        bool addUser(std::string userName, int sock, std::string channelName);
        bool addUser(user *newUser, std::string channelName);

        bool removeUserFromChannel(std::string userName, std::string channelName);

        user *findUserInChannel(std::string userName, std::string channelName);

    private:

    channel *head;
};

#endif // CHANNELLIST_H
