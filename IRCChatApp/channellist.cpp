#include "channellist.h"

ChannelList::ChannelList()
{
    head = new channel();
    head->name = "default";
    head->next = nullptr;
}

ChannelList::~ChannelList()
{

}

bool ChannelList::addChannel(std::string name)
{
    channel *temp = head;

    while(temp->next != nullptr)
    {
        temp = temp->next;
    }

    temp->next = new channel();
    temp->next->name = name;
    temp->next->next = nullptr;

    return true;
}

bool ChannelList::removeChannel(std::string name)
{
    channel *temp = head;
    channel *tempDel = head;

    while(temp->next != nullptr)
    {
        if(temp->next->name == name)
        {
            tempDel = temp->next;
            temp->next = temp->next->next;
            delete tempDel;

            return true;
        }

        temp = temp->next;
    }

    return false;
}

channel *ChannelList::findChannel(std::string name)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == name)
        {

            return temp;
        }

        temp = temp->next;
    }

    return nullptr;
}

bool ChannelList::addUser(std::string userName, std::string ip, std::string channelName)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == channelName)
        {
            user *newUser = new user();
            newUser->name = userName;
            newUser->ip = ip;

            temp->members.push_back(newUser);

            return true;
        }
    }

    return false;
}

bool ChannelList::addUser(user *newUser, std::string channelName)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == channelName)
        {
            temp->members.push_back(newUser);

            return true;
        }
    }

    return false;
}


user *ChannelList::findUserInChannel(std::string userName, std::string channelName)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == channelName)
        {
            for(unsigned int i = 0; i < temp->members.size(); i++)
            {
                if(temp->members.at(i)->name == userName)
                {
                    return temp->members.at(i);
                }
            }
        }
    }

    return nullptr;
}
