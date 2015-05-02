#pragma once

#include "stdafx.h"
#include "Message.h"
#include <queue>

class IMessageSource
{
public:
	virtual ~IMessageSource() {};

	virtual Message getNext() = 0;
	virtual std::queue<Message> getAll() = 0;
};

