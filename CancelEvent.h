#pragma once
#include "Event.h"
#include "Definitions.h"

class CancelEvent : public Event
{
private:

public:
	CancelEvent(EventInfo Input);
	EventInfo Execute();
};

