#include "MessageBuilder.h"


MessageBuilder::MessageBuilder()
{
}


MessageBuilder::~MessageBuilder()
{
}

MessageBuilder *MessageBuilder::putInt(std::string tag, int num)
{
	intsToPut.push_back(num);
	intTags.push_back(tag);
	return this;
}

MessageBuilder *MessageBuilder::putDouble(std::string tag, double num)
{
	doublesToPut.push_back(num);
	doubleTags.push_back(tag);
	return this;
}

MessageBuilder *MessageBuilder::putString(std::string tag, std::string s)
{
	//Clean s of special characters
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '"')
		{
			s[i] = QUOTE_ESCAPE;
		}
	}

	stringsToPut.push_back(s);
	stringTags.push_back(tag);
	return this;
}

MessageBuilder *MessageBuilder::putVector(std::string tag, std::vector<std::string> v)
{
	vectorsToPut.push_back(v);
	vectorTags.push_back(tag);
	return this;
}

MessageBuilder *MessageBuilder::putMessage(std::string tag, Message message)
{
	messagesToPut.push_back(message);
	messageTags.push_back(tag);
	return this;
}

Message MessageBuilder::build()
{
	std::string message = "";

	int totalSize = intsToPut.size() + doublesToPut.size() + stringsToPut.size() + vectorsToPut.size() + messagesToPut.size();

	//ensure that there is at least one thing to put in the message, otherwise return an empty message
	if (totalSize > 0)
	{
		//Put in the leading brace
		message += '{';

		while (intsToPut.size() > 0)
		{
			//Opening quote on tag
			message += '"';
			//Tag
			message += intTags.front();
			//Get rid of the used tag
			intTags.erase(intTags.begin());
			//Closing quote on tag, colon, opening quote on value
			message += "\":\"";
			//Value
			message += std::to_string(intsToPut.front());
			//Get rid of the used value
			intsToPut.erase(intsToPut.begin());
			//Closing quote on value
			message += '"';
		}

		while (doublesToPut.size() > 0)
		{
			//Opening quote on tag
			message += '"';
			//Tag
			message += doubleTags.front();
			//Get rid of the used tag
			doubleTags.erase(doubleTags.begin());
			//Closing quote on tag, colon, opening quote on value
			message += "\":\"";
			//Value
			message += std::to_string(doublesToPut.front());
			//Get rid of the used value
			doublesToPut.erase(doublesToPut.begin());
			//Closing quote on value
			message += '"';
		}

		while (stringsToPut.size() > 0)
		{
			//Opening quote on tag
			message += '"';
			//Tag
			message += stringTags.front();
			//Get rid of the used tag
			stringTags.erase(stringTags.begin());
			//Closing quote on tag, colon, opening quote on value
			message += "\":\"";
			//Value
			message += stringsToPut.front();
			//Get rid of the used value
			stringsToPut.erase(stringsToPut.begin());
			//Closing quote on value
			message += '"';
		}

		while (vectorsToPut.size() > 0)
		{
			//Opening quote on tag
			message += '"';
			//Tag
			message += vectorTags.front();
			//Get rid of the used tag
			vectorTags.erase(vectorTags.begin());
			//Closing quote on tag, colon, opening bracket on value
			message += "\":[";
			//Value
			for (int i = 0; i < vectorsToPut.at(0).size(); i++)
			{
				//Opening quote on sub value
				message += '"';
				//Sub value
				message += vectorsToPut.at(0).at(i);
				//Closing quote on sub value, comma to separate values
				message += "\",";
			}

			//Get rid of the used value
			vectorsToPut.erase(vectorsToPut.begin());
			//Closing bracket on value
			message += ']';
		}

		while (messagesToPut.size() > 0)
		{
			//Opening quote on tag
			message += '"';
			//Tag
			message += messageTags.front();
			//Get rid of the used tag
			messageTags.erase(messageTags.begin());
			//Closing quote on tag, colon, opening quote on value
			message += "\":\"";
			//Value
			message += messagesToPut.front().getRaw();
			//Get rid of the used value
			messagesToPut.erase(messagesToPut.begin());
			//Closing quote on value
			message += '"';
		}

		//Closing brace
		message += '}';

	}

	Message m;
	m.setMessage(message);
	return m;
}
