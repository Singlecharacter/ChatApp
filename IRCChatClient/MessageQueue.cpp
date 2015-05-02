#include "MessageQueue.h"


MessageQueue::MessageQueue()
{
	_source = nullptr;
}

MessageQueue::MessageQueue(IMessageSource *source)
{
	_source = source;
}

MessageQueue::~MessageQueue()
{

}

void MessageQueue::setSource(IMessageSource *source)
{
	_source = source;
}

void MessageQueue::consumeSource(bool clearQueue)
{
	//If the queue is to be cleared, just empty it and replace it with the new one
	if (clearQueue)
	{
		if (_source != nullptr)
		{
			clear();
			_queue = (_source->getAll());
		}
	}
	else
	{
		if (_source != nullptr)
		{
			std::queue<Message> holdQueue = _source->getAll();

			while (holdQueue.size() > 0)
			{
				_queue.push(holdQueue.front());
				holdQueue.pop();
			}
		}
	}
}

std::queue<Message> MessageQueue::getAll(bool remove)
{
	return _queue;
}

Message MessageQueue::getNext(bool remove)
{
	Message m;

	if (_queue.size() > 0)
	{
		m = _queue.front();

		if (remove)
		{
			_queue.pop();
		}
	}

	return m;
}

void MessageQueue::clear()
{
	while (_queue.size() > 0)
	{
		_queue.pop();
	}
}

int MessageQueue::size()
{
	return _queue.size();
}