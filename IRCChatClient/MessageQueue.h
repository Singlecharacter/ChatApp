#pragma once

#include "stdafx.h"
#include "IMessageSource.h"
#include "Message.h"
#include <queue>

class MessageQueue
{
public:

	MessageQueue();
	MessageQueue(IMessageSource *source);
	~MessageQueue();

	//Assigns a source to get messages from
	void setSource(IMessageSource *source);

	//consumeSource consumes all messages in mSource and puts them in mQueue
	void consumeSource(bool clearQueue = false);

	//Methods to get messages from the queue. Flag 'remove' removes the retrieved messages from the queue if set
	std::queue<Message> getAll(bool remove);
	Message getNext(bool remove);

	//Empties the queue
	void clear();

	//Returns the size of the internal queue
	int size();

private:

	IMessageSource *_source;
	std::queue<Message> _queue;

};