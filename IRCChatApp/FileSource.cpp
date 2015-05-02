#include "FileSource.h"


FileSource::FileSource(std::string fname)
{
	_fname = fname;
}


FileSource::~FileSource()
{

}

Message FileSource::getNext()
{
	Message m;

	if (_file.is_open())
	{
		std::string message = "";
		int bracesCount = 0;
		while (_file.good())
		{
			char c = _file.get();
			message += c;
			if (c == '{')
			{
				bracesCount++;
			}
			else if (c == '}')
			{
				bracesCount--;
				if (bracesCount == 0)
				{
					break;
				}
			}
		}

		m.setMessage(message);
	}

	return m;
}

std::queue<Message> FileSource::getAll()
{
	std::queue<Message> queue;
	if (_file.is_open())
	{
		_file.clear();
		_file.seekg(_file.beg);
		while (_file.good())
		{
			Message m = getNext();
			if (m.getRaw().length() > 2)
			{
				queue.push(m);
			}
		}
	}

	return queue;
}

void FileSource::setFilename(std::string fname)
{
	_fname = fname;
}

bool FileSource::openFile()
{
	if (_file.is_open())
	{
		_file.close();
	}

	_file.open(_fname.c_str(), std::ios_base::in);
	return _file.is_open();
}

void FileSource::closeFile()
{
	_file.close();
}
