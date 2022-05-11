#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
#include "Definitions.h"
#include "Node.h"
 #include "LinkedQueue.h"

#include "Event.h"
#include "FormulationEvent.h"
#include "CancelEvent.h"
#include "PromotionEvent.h"
#include "Mission.h"
#include "Rover.h"
#include "UI.h"


class MarsStation
{
private:
	int currentDay = 1;
	int AutoP;
	int nAutoP;
	int ModeType;
	string outputFilename;
	string file;

	bool exit = false;
	bool exitFile = false;

	bool ERfound = false;
	bool MRfound = false;
	bool PRfound = false;
	bool EMfound = false;
	bool MMfound = false;
	bool PMfound = false;

	LinkedQueue<Rover> MRovers;
	LinkedQueue<Rover> ERovers;
	LinkedQueue<Rover> PRovers;
	LinkedQueue<Rover> ICRovers;
	LinkedQueue<Rover> IMRovers;

	LinkedQueue<Event*> Events;

	LinkedQueue<Mission> WEMissions;
	LinkedQueue<Mission> WMMissions;
	LinkedQueue<Mission> WPMissions;
	LinkedQueue<Mission> IEMissions;
	LinkedQueue<Mission> CMissions;

	UI UserInterface;

public:
	int getCurrentDay() const;
	void incCurrentDay();
	int GetMissionType(char input);
	void FormatDataInput(ifstream &filename);
	void ReadDataInput(string filename);
	void CheckForCorner(bool& exit);
	bool CheckForEvents();
	MissionInfo CreateMissionInfo(EventInfo input);
	void CreateMission(MissionInfo input);
	bool CancelMMission(MissionInfo input);
	void ChangeMtoE(MissionInfo& input);
	bool PromoteMMission(MissionInfo input);
	void EventsOperating();
	void ReturnRover(Rover input);
	void ReturnMission(Mission input);
	void CheckIE();
	void AutoPromotionMountainous();
	void MissionAssignment();
	void IncMissionsWaiting();
	bool checkRandomness();
	void MissionsOperating();
	void CheckForIC();
	void CheckForICOut();
	void CheckForMaintenance();
	void CheckForMaintenanceOut();
	void RoversOperating();
	string WMString();
	string IEMString();
	string ARString();
	string ICRString();
	string IMRString();
	string CMString();
	string FormatOutput();
	void CheckForMode();
	bool CheckForExit();
	void StartUP();
	void Operating();
	void CreateOutputFile();
};