#pragma once

#include "Message.h"
#include <vector>

/*
 * MessageBuilder is a class for building a message from name-value pairs.
 * Its methods ensure that information is encoded in the correct format. All messages
 * should be initially created with MessageBuilder to ensure that they are in the
 * correct format.
 * Messages support integers, doubles, strings, lists of strings, and other Messages.
 */

class MessageBuilder
{
public:
	MessageBuilder();
	~MessageBuilder();

	MessageBuilder *putInt(std::string tag, int num);
	MessageBuilder *putDouble(std::string tag, double num);
	MessageBuilder *putString(std::string tag, std::string s);
	MessageBuilder *putVector(std::string tag, std::vector<std::string> v);
	MessageBuilder *putMessage(std::string tag, Message message);
	Message build();

private:

	std::vector<int> intsToPut;
	std::vector<double> doublesToPut;
	std::vector<std::string> stringsToPut;
	std::vector<std::vector<std::string> > vectorsToPut;
	std::vector<Message> messagesToPut;

	std::vector<std::string> intTags;
	std::vector<std::string> doubleTags;
	std::vector<std::string> stringTags;
	std::vector<std::string> vectorTags;
	std::vector<std::string> messageTags;
};

