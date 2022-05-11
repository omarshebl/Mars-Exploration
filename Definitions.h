#pragma once
#ifndef _DEF_H
#define _DEF_H
// FD, Formulation Days
// WD, Waiting Days
// ED, Execution Days
// CD, Completion Days

enum UIMode {
	Interactive = 1,
	StepByStep,
	Silent
};


enum RoverType {
	EmergencyRover,
	MountainousRover,
	PolarRover
};

enum MissionType {
	EmergencyMission = 1,
	MountainousMission,
	PolarMission
};

enum EventType {
	FormEventT = 1,
	CancEventT,
	PromoEventT
};

struct RoverInfo {
	int ID;
	int Type; // EmergencyRover or MountainousRover or PolarRover.
	int CUD; // Checkup Duration in days.
	int CUDn; // Checkup needed after N missions.
	int Speed; // Speed of Rover in km/hour.
	int Health = 100; // Health of Rover 0 to 100.
};

struct MissionInfo {
	int ID;
	int FD; // Day when mission is ready to be assigned.
	int MissionType; // EmergencyMission or MountainousMission or PolarMission
	int TargetLocation; // Distance in km from base station to target location.
	int MissionDuration; // Duration of mission in days.
	int SIG; // Number representing significance, higher is more important.
};

struct EventInfo {
	int ED;
	int ID;
	int Type=0;
	int MissionT=0;
	int TLOC=0;
	int MDUR=0;
	int SIG=0;
};

#endif

