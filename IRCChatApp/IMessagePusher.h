#pragma once

#include "stdafx.h"
#include "Message.h"
#include "MessageQueue.h"
#include "IMessageDestination.h"

class IMessagePusher
{
public:

	virtual ~IMessagePusher() {};

	virtual void setDestination(IMessageDestination *dest) = 0;

	virtual bool writeMessage(Message *message) = 0;
	virtual bool writeAll(MessageQueue queue) = 0;
};

