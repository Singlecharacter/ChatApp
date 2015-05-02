#include "FilePusher.h"


FilePusher::FilePusher()
{
	_dest = NULL;
}

FilePusher::FilePusher(FileDestination *dest)
{
	_dest = dest;
}

FilePusher::~FilePusher()
{

}

void FilePusher::setDestination(IMessageDestination *dest)
{
	_dest = (FileDestination*) dest;
}

bool FilePusher::writeMessage(Message *m)
{
	if (_dest != NULL)
	{
		if (_dest->openFile())
		{
			_dest->write(m);
			_dest->closeFile();
			return true;
		}
	}
	
	return false;
}

bool FilePusher::writeAll(MessageQueue queue)
{
	if (!_dest->openFile())
	{
		return false;
	}

	std::string allMessages = "";

	while (queue.size() > 0)
	{
		allMessages += queue.getNext(true).getRaw();
	}

	bool writeOK = _dest->write(new Message(allMessages));
	_dest->closeFile();

	if (!writeOK)
	{
		return false;
	}

	return true;
}
