#pragma once

#include "Message.h"

class IMessageDestination
{
public:
	virtual ~IMessageDestination() {};

	virtual bool write(Message *m) = 0;
};

