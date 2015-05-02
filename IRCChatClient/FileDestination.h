#pragma once

#include "IMessageDestination.h"
#include <fstream>

class FileDestination : public IMessageDestination
{
public:

	FileDestination(std::string fname);
	FileDestination();
	~FileDestination();

	bool openFile();
	void closeFile();

	void setFilename(std::string fname);

	bool write(Message *m);

private:

	std::string _fname;
	std::ofstream _file;
};

