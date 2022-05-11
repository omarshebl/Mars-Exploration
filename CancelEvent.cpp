#include "CancelEvent.h"
#include "Definitions.h"

CancelEvent::CancelEvent(EventInfo Input) : Event(Input.ED, Input.ID, Input.Type)
{
}

EventInfo CancelEvent::Execute()
{
	EventInfo Temp;
	Temp.ID = ID;
	Temp.ED = ED;
	Temp.Type = type;
	return Temp;
}
