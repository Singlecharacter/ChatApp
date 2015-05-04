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

    std::cout << temp->next->name << std::endl;

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

bool ChannelList::addUser(std::string userName, int sock, std::string channelName)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == channelName)
        {
            user *newUser = new user();
            newUser->name = userName;
            newUser->sock = sock;

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

        temp = temp->next;
    }

    return false;
}

bool ChannelList::removeUserFromChannel(std::string userName, std::string channelName)
{
    channel *temp = head;

    while(temp != nullptr)
    {
        if(temp->name == channelName)
        {
            for(int i = 0; i < temp->members.size(); i++)
            {
                if(temp->members.at(i)->name == userName)
                {
                    temp->members.erase(temp->members.begin()+i);
                    return true;
                }
            }

            return false;
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
