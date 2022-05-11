#include "Rover.h"

RoverInfo Rover::getInfo()
{
	return Data;
}

int Rover::getID() const
{
	return Data.ID;
}

void Rover::setInfo(RoverInfo dataInput)
{
	Data = dataInput;
	hSpeed = dataInput.Speed / 2;
}

int Rover::getTMission() const
{
	return TMissions;
}

int Rover::getNMission() const
{
	return NMissions;
}

void Rover::incTNMission()
{
	TMissions++;
	NMissions++;
}

bool Rover::checkNMissions()
{
	if (NMissions >= Data.CUDn)
		return true;
	return false;
}

void Rover::resetNMissions()
{
	NMissions = 0;
}

bool Rover::checkKT()
{
	if (KilometersTravelled >= 2000) {
		return true;
	}
	return false;
}

void Rover::decKT()
{
	KilometersTravelled -= 1000;
}

void Rover::decKT(int input)
{
	KilometersTravelled /= input;
}

void Rover::incKT(int input)
{
	KilometersTravelled += input;
}

void Rover::setCD(int input)
{
	CheckupDuration = Data.CUD + input;
}

int Rover::getCD() const
{
	return CheckupDuration;
}

void Rover::incDO(int input)
{
	DaysOnline += input;
}

int Rover::getSpeed() const
{
	return Data.Speed;
}

void Rover::decSpeed()
{
	Data.Speed = hSpeed;
}
