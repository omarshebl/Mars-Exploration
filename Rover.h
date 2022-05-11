#pragma once
#include "Definitions.h"
class Rover
{
private:
	RoverInfo Data;
	float hSpeed;
	int TMissions = 0;
	int NMissions = 0;
	int KilometersTravelled = 0;
	int CheckupDuration = 0;
	int DaysOnline = 0;

public:
	RoverInfo getInfo();
	int getID() const;
	void setInfo(RoverInfo dataInput);
	int getTMission() const;
	int getNMission() const;
	void incTNMission();
	bool checkNMissions();
	void resetNMissions();
	bool checkKT();
	void decKT();
	void decKT(int);
	void incKT(int input);
	void setCD(int input);
	int getCD() const;
	void incDO(int input);
	int getSpeed() const;
	void decSpeed();
};

