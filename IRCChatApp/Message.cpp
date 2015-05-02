#include "Message.h"
#include <math.h>


Message::Message()
{
	_message = "{\"tag\":\"NO_MESSAGE\"}";
}

Message::Message(std::string message)
{
	std::string cleanedString = cleanString(message);
	if (cleanedString == "")
	{
		_message = "{\"tag\":\"NO_MESSAGE\"}";
	}
	else
	{
		_message = cleanedString;
	}
}


Message::~Message()
{

}

void Message::setMessage(std::string message)
{
	_message = cleanString(message);
}

int Message::getInt(std::string tag)
{
	std::string toConvert = findWithTag(tag, '"');

	int convertedInt = 0;

	while (toConvert.length() > 0)
	{
		convertedInt = convertedInt * 10;

		int toAdd = 0;

		char c = toConvert.at(0);
		toConvert.erase(0, 1);

		switch (c)
		{
			case '0':
				toAdd = 0;
				break;
			case '1':
				toAdd = 1;
				break;
			case '2':
				toAdd = 2;
				break;
			case '3':
				toAdd = 3;
				break;
			case '4':
				toAdd = 4;
				break;
			case '5':
				toAdd = 5;
				break;
			case '6':
				toAdd = 6;
				break;
			case '7':
				toAdd = 7;
				break;
			case '8':
				toAdd = 8;
				break;
			case '9':
				toAdd = 9;
				break;
			default:
				break;
		}

		convertedInt += toAdd;
	}

	return convertedInt;
}

double Message::getDouble(std::string tag)
{
	std::string toConvert = findWithTag(tag, '"');

	double leadingDigits = 0;
	double trailingDigits = 0;

	//Get all digits in front of the decimal place and convert them to a number
	while (toConvert.length() > 0)
	{
		std::cout << toConvert.length() << std::endl;
		if (toConvert.at(0) == '.')
		{
			toConvert.erase(0, 1);
			break;
		}
		else
		{
			leadingDigits = leadingDigits * 10;

			double toAdd = 0;
			switch (toConvert.at(0))
			{
				case '0':
					toAdd = 0;
					break;
				case '1':
					toAdd = 1;
					break;
				case '2':
					toAdd = 2;
					break;
				case '3':
					toAdd = 3;
					break;
				case '4':
					toAdd = 4;
					break;
				case '5':
					toAdd = 5;
					break;
				case '6':
					toAdd = 6;
					break;
				case '7':
					toAdd = 7;
					break;
				case '8':
					toAdd = 8;
					break;
				case '9':
					toAdd = 9;
					break;
				default:
					break;
			}

			toConvert.erase(0, 1);

			leadingDigits += toAdd;
		}
	}

	//to divide by the right number when adding the new digit
	float decimalPlaces = 1;

	//Get all digits behind the decimal place and convert them to a number < 1
	while (toConvert.length() > 0)
	{
		double toAdd = 0;

		switch (toConvert.at(0))
		{
			case '0':
				toAdd = 0;
				break;
			case '1':
				toAdd = 1;
				break;
			case '2':
				toAdd = 2;
				break;
			case '3':
				toAdd = 3;
				break;
			case '4':
				toAdd = 4;
				break;
			case '5':
				toAdd = 5;
				break;
			case '6':
				toAdd = 6;
				break;
			case '7':
				toAdd = 7;
				break;
			case '8':
				toAdd = 8;
				break;
			case '9':
				toAdd = 9;
				break;
			default:
				break;
		}

		toConvert.erase(0, 1);

		trailingDigits += toAdd / pow(10,decimalPlaces);

		decimalPlaces++;
	}

	//Return the sum of the leading digits and trailing digits
	return leadingDigits + trailingDigits;
}

Message Message::getMessage(std::string tag)
{
	return findWithTag(tag, '}');
}

std::string Message::getString(std::string tag)
{
	return findWithTag(tag, '"');
}

std::vector<std::string> Message::getVector(std::string tag)
{
	std::string vecString = findWithTag(tag, ']');

	std::vector<std::string> outVec;
	
	for (int i = 0; i < vecString.length(); i++)
	{
		if (vecString[i] == ',')
		{
			std::string s = vecString.substr(1, i - 2);
			outVec.push_back(s);
			vecString.erase(0, i+1);
			i = 0;
		}
	}

	return outVec;
}

std::string Message::getRaw()
{
	return _message;
}

std::string Message::findWithTag(std::string tag, char delimiter)
{
	for (int i = 0; i < _message.length()-tag.length() - 1; i++)
	{
		//Find the tag in the message
		if (_message.substr(i, tag.length()) == tag && (_message.at(i - 1) == '"') && _message.at(i + tag.length()) == '"')
		{
			//Find the end of the value associated with that tag, determined by delimiter
			int j = 0;
			for (j = i + tag.length()+3; j < _message.length(); j++)
			{
				if (_message.at(j) == delimiter)
				{
					break;
				}
			}

			//If the end of the value was found, return the value
			if (j < _message.length())
			{
				i = i + tag.length() + 3;
				return _message.substr(i, j - i);
			}
		}
	}

	return "";
}

std::string Message::cleanString(std::string s)
{
	bool inQuotes = false;
	for (int i = 0; i < s.length(); i++)
	{
		if ((s.at(i) == ' ' || s.at(i) == '\n' || s.at(i) == '\t') && !inQuotes)
		{
			s.erase(i, 1);
			i--;
		}
		else if (s.at(i) == '"')
		{
			inQuotes = !inQuotes;
		}
	}

	return s;
}
