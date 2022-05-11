#include "PromotionEvent.h"
#include "Definitions.h"
PromotionEvent::PromotionEvent(EventInfo Input) : Event(Input.ED, Input.ID, Input.Type)
{
}

EventInfo PromotionEvent::Execute()
{
	EventInfo Temp;
	Temp.ID = ID;
	Temp.ED = ED;
	Temp.Type = type;
	return Temp;
}
