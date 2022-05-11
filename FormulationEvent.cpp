#include "FormulationEvent.h"
#include "Definitions.h"
FormulationEvent::FormulationEvent(EventInfo Input) : Event(Input.ED, Input.ID, Input.Type)
{
	MissionT = Input.MissionT;
	TLOC = Input.TLOC;
	MDUR = Input.MDUR;
	SIG = Input.SIG;
}


EventInfo FormulationEvent::Execute()
{
	EventInfo Temp;
	Temp.ID = ID;
	Temp.ED = ED;
	Temp.Type = type;
	Temp.MissionT = MissionT;
	Temp.MDUR = MDUR;
	Temp.TLOC = TLOC;
	Temp.SIG = SIG;
	return Temp;
}