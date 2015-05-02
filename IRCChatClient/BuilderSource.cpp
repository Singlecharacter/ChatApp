#include "BuilderSource.h"

BuilderSource::BuilderSource(int length)
{
    for(int i = 0; i < length; i++)
    {
        _queue.push((new MessageBuilder())->putString("tag","NO_MESSAGE")->putDouble("num",2.314512)->build());
    }
}

BuilderSource::~BuilderSource(){};

Message BuilderSource::getNext()
{
    Message m = _queue.front();
    _queue.pop();

    return m;
}

std::queue<Message> BuilderSource::getAll()
{
    return _queue;
}
