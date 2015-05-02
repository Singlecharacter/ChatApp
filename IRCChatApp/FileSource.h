#pragma once

#include "IMessageSource.h"
#include <fstream>

class FileSource :
	public IMessageSource
{
public:
	FileSource(std::string fname);
	~FileSource();

	Message getNext();
	std::queue<Message> getAll();

	void setFilename(std::string fname);

	bool openFile();
	void closeFile();

private:

	std::string _fname;
	std::ifstream _file;
};

