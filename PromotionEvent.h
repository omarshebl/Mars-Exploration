#pragma once
#include "Event.h"
#include "Definitions.h"

class PromotionEvent : public Event
{
private:

public:
	PromotionEvent(EventInfo Input);
	EventInfo Execute();
};

