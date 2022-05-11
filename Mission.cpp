#include "Mission.h"


void Mission::setInfo(MissionInfo input)
{
	ID = input.ID;
	FD = input.FD;
	MissionT = input.MissionType;
	TLOC = input.TargetLocation;
	MDUR = input.MissionDuration;
	SIG = input.SIG;
}

MissionInfo Mission::getInfo() const
{
	MissionInfo output;
	output.ID = ID;
	output.FD = FD;
	output.MissionType = MissionT;
	output.TargetLocation = TLOC;
	output.MissionDuration = MDUR;
	output.SIG = SIG;
	return output;
}

void Mission::setRover(Rover input)
{
	MissionRover = input;
}

int Mission::getRoverID() const
{
	return MissionRover.getID();
}

Rover Mission::getRover()
{
	Rover ouput = MissionRover;
	MissionRover.~Rover();
	return ouput;
}

int Mission::getWD() const
{
	return WD;
}

void Mission::incWD()
{
	WD++;
}

float Mission::calculatePriority()
{
	float WDweight = 0.35;
	float TLOCweight = 0.3;
	float SIGweight = 0.2;
	float MDURweight = 0.15;

	float priority = (WD * WDweight + SIG * SIGweight + (1.0/TLOC) * TLOCweight + (1.0/MDUR) * MDURweight) * 100;

	return priority;
}

bool Mission::checkOverStress()
{
	if (TLOC >= 500 && MDUR >= 6 && MissionRover.checkKT()) return true;
	return false;
}

int Mission::getCD() const
{
	return CD;
}

int Mission::getED() const
{
	return ED;
}

int Mission::getID() const
{
	return ID;
}

int Mission::getFD() const
{
	return FD;
}

void Mission::MissionTransferedIE()
{
	ED = ceil(((float)TLOC) / (MissionRover.getSpeed() * 25)) * 2 + MDUR;
	CD = FD + WD + ED;
	MissionRover.incKT(TLOC * 2);
	MissionRover.incTNMission();
	MissionRover.incDO(ED);
}

