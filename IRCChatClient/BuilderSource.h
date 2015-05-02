#pragma once

#include "stdafx.h"
#include <queue>
#include "MessageBuilder.h"
#include "IMessageSource.h"

class BuilderSource : public IMessageSource
{

public:

    BuilderSource(int length);
    ~BuilderSource();

    Message getNext();

    std::queue<Message> getAll();

private:

    std::queue<Message> _queue;
};
