#include "FileDestination.h"


FileDestination::FileDestination()
{
}

FileDestination::FileDestination(std::string fname)
{
	_fname = fname;
}


FileDestination::~FileDestination()
{
}

bool FileDestination::openFile()
{
	if (_file.is_open())
	{
		_file.close();
	}

	_file.open(_fname.c_str(),std::iostream::app);

	return _file.is_open();
}

void FileDestination::closeFile()
{
	_file.close();
}

void FileDestination::setFilename(std::string fname)
{
	_fname = fname;
}

bool FileDestination::write(Message *m)
{
	if (_file.is_open())
	{
		_file << m->getRaw();
		return true;
	}
	else
	{
		return false;
	}
}
