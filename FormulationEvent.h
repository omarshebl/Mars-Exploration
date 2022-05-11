#pragma once
#include "Event.h"
#include "Definitions.h"

class FormulationEvent : public Event
{
private:
	int MissionT, TLOC, MDUR, SIG;

public:
	FormulationEvent(EventInfo input);
	EventInfo Execute();
};

