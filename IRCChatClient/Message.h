#pragma once

#include "stdafx.h"
#include <string>
#include <vector>

class Message
{
public:

	Message(std::string message);
	Message();
	~Message();

	void setMessage(std::string message);

	int getInt(std::string tag);
	double getDouble(std::string tag);
	Message getMessage(std::string tag);
	std::string getString(std::string tag);
	std::vector<std::string> getVector(std::string tag);
	std::string getRaw();

private:

	std::string _message;

	std::string findWithTag(std::string tag, char delimiter);
	Message findMessageWithTag(std::string tag);
	std::string cleanString(std::string s);
};

