#pragma once
#include "Definitions.h"
#include "Rover.h"
#include <cmath>

class Mission
{
private:
	int ID, FD, MissionT, TLOC, MDUR, SIG;
	
	int WD = 0;
	int ED = 0;
	int CD = 0;

	Rover MissionRover;
public:
	void setInfo(MissionInfo input);
	MissionInfo getInfo() const;

	void setRover(Rover input);
	int getRoverID() const;
	Rover getRover();


	int getWD() const;
	int getCD() const;
	int getED() const;
	int getID() const;
	int getFD() const;
	void incWD();

	float calculatePriority(); // For Emergency Missions use only
	bool checkOverStress();

	void MissionTransferedIE();
};

