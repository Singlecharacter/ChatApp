#pragma once

#include "IMessagePusher.h"
#include "FileDestination.h"

class FilePusher :
	public IMessagePusher
{
public:

	FilePusher();
	FilePusher(FileDestination *dest);
	~FilePusher();

	void setDestination(IMessageDestination *dest);

	bool writeMessage(Message *message);


	bool writeAll(MessageQueue queue);

private:

	FileDestination *_dest;
};

