#include "MarsStation.h"

int MarsStation::getCurrentDay() const
{
	return currentDay;
}

void MarsStation::incCurrentDay()
{
	currentDay++;
}

int MarsStation::GetMissionType(char input)
{
	if (input == 'M') {
		return MountainousMission;
	}
	else if (input == 'P') {
		return PolarMission;
	}
	else if (input == 'E') {
		return EmergencyMission;
	}
	return 0;
}

void MarsStation::FormatDataInput(ifstream& filename)
{
	// Read Rover Quantities
	int ERn, MRn, PRn;
	filename >> MRn >> PRn >> ERn ;

	if (ERn > 0) ERfound = true;
	if (MRn > 0) MRfound = true;
	if (PRn > 0) PRfound = true;

	int SpeedInputType; // 0 for comulative, 1 for indiviual
	filename >> SpeedInputType;

	int* nm = new int[ERn + MRn + PRn];

	int ERs, MRs, PRs;

	if (SpeedInputType) {
		for (int i = 0; i < (ERn + MRn + PRn); i++) {
			filename >> nm[i];
		}
	}
	else {
		// Read Speeds of each Rover Type
		filename >> MRs >> PRs >> ERs;
	}

	// Read # of missions before checkup and the Checkup duration of each Rover Type
	int CUDn, ERc, MRc, PRc;
	filename >> CUDn >> ERc >> MRc >> PRc;

	if (SpeedInputType) {
		int ID = 1;
		RoverInfo R;
		R.Type = MountainousRover;
		R.CUDn = CUDn;
		R.CUD = MRc;
		for (int i = 0; i < MRn; i++) {
			Rover x;
			R.ID = ID;
			R.Speed = nm[i];
			ID++;
			x.setInfo(R);
			MRovers.enqueue(x);
		}
		R.Type = PolarRover;
		R.CUDn = CUDn;
		R.CUD = PRc;
		for (int i = 0; i < PRn; i++) {
			Rover x;
			R.ID = ID;
			R.Speed = nm[i + MRn];
			ID++;
			x.setInfo(R);
			PRovers.enqueue(x);
		}
		R.Type = EmergencyRover;
		R.CUDn = CUDn;
		R.CUD = ERc;
		for (int i = 0; i < ERn; i++) {
			Rover x;
			R.ID = ID;
			R.Speed = nm[i + MRn + PRn];
			ID++;
			x.setInfo(R);
			ERovers.enqueue(x);
		}
	}
	else {
		int ID = 1;
		RoverInfo R;
		R.Type = EmergencyRover;
		R.CUDn = CUDn;
		R.CUD = ERc;
		R.Speed = ERs;
		for (int i = 0; i < ERn; i++) {
			Rover x;
			R.ID = ID;
			ID++;
			x.setInfo(R);
			ERovers.enqueue(x);
		}

		R.Type = MountainousRover;
		R.CUDn = CUDn;
		R.CUD = MRc;
		R.Speed = MRs;
		for (int i = 0; i < MRn; i++) {
			Rover x;
			R.ID = ID;
			ID++;
			x.setInfo(R);
			MRovers.enqueue(x);
		}
		R.Type = PolarRover;
		R.CUDn = CUDn;
		R.CUD = PRc;
		R.Speed = PRs;
		for (int i = 0; i < PRn; i++) {
			Rover x;
			R.ID = ID;
			ID++;
			x.setInfo(R);
			PRovers.enqueue(x);
		}
	}

	// Read Auto Promotion Limit
	filename >> AutoP;

	// Read # of events
	int nOfEvents;
	filename >> nOfEvents;

	for (int i = 0; i < nOfEvents; i++) {
		char EventT;
		filename >> EventT;
		if (EventT == 'F') {
			EventInfo Data;
			Data.Type = FormEventT;
			char x;
			filename >> x >> Data.ED >> Data.ID >> Data.TLOC >> Data.MDUR >> Data.SIG;
			Data.MissionT = GetMissionType(x);
			if (Data.MissionT == EmergencyMission) EMfound = true;
			if (Data.MissionT == MountainousMission) MMfound = true;
			if (Data.MissionT == PolarMission) PMfound = true;
			FormulationEvent* T = new FormulationEvent(Data);
			Events.enqueue(T);
		}
		else if (EventT == 'X') {
			EventInfo Data;
			Data.Type = CancEventT;
			filename >> Data.ED >> Data.ID;
			CancelEvent* T = new CancelEvent(Data);
			Events.enqueue(T);
		}
		else if (EventT == 'P') {
			EventInfo Data;
			Data.Type = PromoEventT;
			filename >> Data.ED >> Data.ID;
			PromotionEvent* T = new PromotionEvent(Data);
			Events.enqueue(T);
		}
	}

}

void MarsStation::ReadDataInput(string filename)
{
	ifstream inputFile;
	inputFile.open(filename);
	if (inputFile.is_open()) {
		FormatDataInput(inputFile);
	}
	else {
		exitFile = true;
	}

}

void MarsStation::CheckForCorner(bool& exit)
{
	if (EMfound && !(ERfound || MRfound || PRfound)) exit = true;
	if (MMfound && !(ERfound || MRfound)) exit = true;
	if (PMfound && !(PRfound)) exit = true;
}

void MarsStation::StartUP()
{
	UserInterface.startupAnimation();
	UserInterface.getFilename(file);	
}

bool MarsStation::CheckForEvents()
{
	Event* Test;
	if (!Events.peek(Test))
		return false;

	if (Test->getED() != getCurrentDay())
		return false;

	return true;
}

MissionInfo MarsStation::CreateMissionInfo(EventInfo input)
{
	MissionInfo output;
	output.FD = input.ED;
	output.MissionDuration = input.MDUR;
	output.MissionType = input.MissionT;
	output.SIG = input.SIG;
	output.TargetLocation = input.TLOC;
	output.ID = input.ID;
	return output;
}

void MarsStation::CreateMission(MissionInfo input)
{
	Mission Temp;
	Temp.setInfo(input);
	if (input.MissionType == MountainousMission) {
		WMMissions.enqueue(Temp);
	} else if (input.MissionType == PolarMission) {
		WPMissions.enqueue(Temp);
	} else if (input.MissionType == EmergencyMission) {
		WEMissions.enqueue(Temp);
	}
}

bool MarsStation::CancelMMission(MissionInfo input)
{
	int ID = input.ID;
	return WMMissions.DeleteByID(ID);
}

void MarsStation::ChangeMtoE(MissionInfo& input)
{
	input.MissionType = EmergencyMission;
}

bool MarsStation::PromoteMMission(MissionInfo input)
{
	Mission Transfer;
	if (WMMissions.FindByID(input.ID, Transfer)) {
		MissionInfo newInfo = Transfer.getInfo();
		ChangeMtoE(newInfo);
		Transfer.setInfo(newInfo);
		WEMissions.enqueue(Transfer);
		return true;
	}
	return false;
}

void MarsStation::EventsOperating()
{
	while (CheckForEvents()) {
		Event* Temp;
		Events.dequeue(Temp);
		int type = Temp->gettype();
		if (type == FormEventT) {
			EventInfo info = dynamic_cast<FormulationEvent*>(Temp)->Execute();
			MissionInfo InfoTemp = CreateMissionInfo(info);
			CreateMission(InfoTemp);
		}
		else if (type == CancEventT) {
			EventInfo info = dynamic_cast<CancelEvent*>(Temp)->Execute();
			MissionInfo InfoTemp = CreateMissionInfo(info);
			CancelMMission(InfoTemp);
		}
		else if (type == PromoEventT) {
			EventInfo info = dynamic_cast<PromotionEvent*>(Temp)->Execute();
			MissionInfo InfoTemp = CreateMissionInfo(info);
			PromoteMMission(InfoTemp);
		}
	}
}

void MarsStation::ReturnRover(Rover input)
{
	if (input.getInfo().Type == MountainousRover) {
		MRovers.enqueue(input);
	} else if (input.getInfo().Type == PolarRover) {
		PRovers.enqueue(input);
	} else if (input.getInfo().Type == EmergencyRover) {
		ERovers.enqueue(input);
	}
}

void MarsStation::ReturnMission(Mission input)
{
	if (input.getInfo().MissionType == MountainousMission) {
		WMMissions.enqueue(input);
	}
	else if (input.getInfo().MissionType == PolarMission) {
		WPMissions.enqueue(input);
	}
	else if (input.getInfo().MissionType == EmergencyMission) {
		WEMissions.enqueue(input);
	}
}

void MarsStation::CheckIE()
{
	if (IEMissions.isEmpty())
		return;

	Mission MTemp;
	LinkedQueue<Mission> Temp;
	while (IEMissions.dequeue(MTemp)) {
		if (MTemp.getCD() == getCurrentDay()) {
			ReturnRover(MTemp.getRover());
			CMissions.enqueue(MTemp);
		}
		else {
			if (MTemp.checkOverStress() && checkRandomness()) {
				Rover RReturn = MTemp.getRover();
				ReturnMission(MTemp);
				RReturn.setCD(getCurrentDay());
				RReturn.resetNMissions();
				ICRovers.enqueue(RReturn);
			}
			else {
				Temp.enqueue(MTemp);
			}
		}
	}

	while (Temp.dequeue(MTemp))
		IEMissions.enqueue(MTemp);
}

void MarsStation::AutoPromotionMountainous()
{
	LinkedQueue<Mission> Temp;
	Mission MTemp;

	while (WMMissions.dequeue(MTemp)) {
		if (MTemp.getWD() >= AutoP) {
			MissionInfo newInfo = MTemp.getInfo();
			ChangeMtoE(newInfo);
			MTemp.setInfo(newInfo);
			WEMissions.enqueue(MTemp);
			nAutoP++;
		}
		else {
			Temp.enqueue(MTemp);
		}
	}

	while (Temp.dequeue(MTemp))
		WMMissions.enqueue(MTemp);
}

void MarsStation::MissionAssignment()
{
	// Starting with waiting emergency missions. HIGH PRIORITY
	while (!WEMissions.isEmpty() && (!MRovers.isEmpty() || !PRovers.isEmpty() || !ERovers.isEmpty() || !IMRovers.isEmpty())) {
		Rover RTransfer;
		Mission MTransfer;
		WEMissions.getHighestPriority(MTransfer);
		if (!ERovers.isEmpty()) {
			ERovers.getFastestRover(RTransfer);
		}
		else if (!MRovers.isEmpty()) {
			MRovers.getFastestRover(RTransfer);
		}
		else if (!PRovers.isEmpty()) {
			PRovers.getFastestRover(RTransfer);
		}
		else {
			IMRovers.getFastestRover(RTransfer);
			RTransfer.decSpeed();
		}
		MTransfer.setRover(RTransfer);
		MTransfer.MissionTransferedIE();
		IEMissions.enqueue(MTransfer);
	}

	// then with waiting mountainous missions. MEDIUM PRIORITY
	while (!WMMissions.isEmpty() && (!MRovers.isEmpty() || !ERovers.isEmpty())) {
		Rover RTransfer;
		Mission MTransfer;
		WMMissions.getHighestSIG(MTransfer);
		if (!MRovers.isEmpty()) {
			MRovers.getFastestRover(RTransfer);
		}
		else {
			ERovers.getFastestRover(RTransfer);
		}
		MTransfer.setRover(RTransfer);
		MTransfer.MissionTransferedIE();
		IEMissions.enqueue(MTransfer);
	}

	// then with waiting POLAR missions. LOW PRIORITY
	while (!WPMissions.isEmpty() && !PRovers.isEmpty()) {
		Rover RTransfer;
		Mission MTransfer;
		WPMissions.getHighestSIG(MTransfer);
		PRovers.getFastestRover(RTransfer);
		MTransfer.setRover(RTransfer);
		MTransfer.MissionTransferedIE();
		IEMissions.enqueue(MTransfer);
	}
}

void MarsStation::IncMissionsWaiting()
{
	Mission MTemp;
	LinkedQueue<Mission> Temp;
	
	while (WEMissions.dequeue(MTemp)) {
		MTemp.incWD();
		Temp.enqueue(MTemp);
	}

	while (Temp.dequeue(MTemp))
		WEMissions.enqueue(MTemp);

	while (WMMissions.dequeue(MTemp)) {
		MTemp.incWD();
		Temp.enqueue(MTemp);
	}

	while (Temp.dequeue(MTemp))
		WMMissions.enqueue(MTemp);

	while (WPMissions.dequeue(MTemp)) {
		MTemp.incWD();
		Temp.enqueue(MTemp);
	}

	while (Temp.dequeue(MTemp))
		WPMissions.enqueue(MTemp);
}

bool MarsStation::checkRandomness()
{
		return ((rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1)
			&& (rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1) 
			&& (rand() & 1) && (rand() & 1));
}

void MarsStation::MissionsOperating()
{	
	CheckIE();
	AutoPromotionMountainous();
	MissionAssignment();
	IncMissionsWaiting();
}

void MarsStation::CheckForIC()
{
	LinkedQueue<Rover> RTemp;
	Rover Temp;
	// Check for emergency rovers that need checkup;
	while (ERovers.dequeue(Temp)) {
		if (Temp.checkNMissions()) {
			Temp.resetNMissions();
			Temp.setCD(getCurrentDay());
			ICRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		ERovers.enqueue(Temp);

	while (MRovers.dequeue(Temp)) {
		if (Temp.checkNMissions()) {
			Temp.resetNMissions();
			Temp.setCD(getCurrentDay());
			ICRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		MRovers.enqueue(Temp);

	while (PRovers.dequeue(Temp)) {
		if (Temp.checkNMissions()) {
			Temp.resetNMissions();
			Temp.setCD(getCurrentDay());
			ICRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		PRovers.enqueue(Temp);

}

void MarsStation::CheckForICOut()
{
	LinkedQueue<Rover> RTemp;
	Rover Temp;

	while (ICRovers.dequeue(Temp)) {
		if (Temp.getCD() < getCurrentDay()) {
			Temp.decKT(2);
			ReturnRover(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		ICRovers.enqueue(Temp);
		
}

void MarsStation::CheckForMaintenance()
{
	LinkedQueue<Rover> RTemp;
	Rover Temp;
	while (ERovers.dequeue(Temp)) {
		if (Temp.checkKT()) {
			Temp.decKT();
			Temp.setCD(getCurrentDay()+5);
			IMRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		ERovers.enqueue(Temp);

	while (MRovers.dequeue(Temp)) {
		if (Temp.checkKT()) {
			Temp.decKT();
			Temp.setCD(getCurrentDay() + 4);
			IMRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		MRovers.enqueue(Temp);

	while (PRovers.dequeue(Temp)) {
		if (Temp.checkKT()) {
			Temp.decKT();
			Temp.setCD(getCurrentDay() + 3);
			IMRovers.enqueue(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		PRovers.enqueue(Temp);
}

void MarsStation::CheckForMaintenanceOut()
{
	LinkedQueue<Rover> RTemp;
	Rover Temp;

	while (IMRovers.dequeue(Temp)) {
		if (Temp.getCD() < getCurrentDay()) {
			ReturnRover(Temp);
		}
		else {
			RTemp.enqueue(Temp);
		}
	}

	while (RTemp.dequeue(Temp))
		IMRovers.enqueue(Temp);
}

void MarsStation::RoversOperating()
{
	CheckForIC();
	CheckForICOut();
	CheckForMaintenance();
	CheckForMaintenanceOut();
}

string MarsStation::WMString()
{
	string output = "";
	string MString = "";
	string PString = "";
	string EString = "";

	LinkedQueue<Mission> Temp;
	Mission MTemp;

	if (!WEMissions.isEmpty()) {
		EString = "[";
		while (WEMissions.dequeue(MTemp)) {
			Temp.enqueue(MTemp);
			EString += to_string(MTemp.getID());
			EString += ", ";
		}
		
		while (Temp.dequeue(MTemp))
			WEMissions.enqueue(MTemp);

		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}

	if (!WPMissions.isEmpty()) {
		PString = "(";
		while (WPMissions.dequeue(MTemp)) {
			Temp.enqueue(MTemp);
			PString += to_string(MTemp.getID());
			PString += ", ";
		}

		while (Temp.dequeue(MTemp))
			WPMissions.enqueue(MTemp);

		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}

	if (!WMMissions.isEmpty()) {
		MString = "{";
		while (WMMissions.dequeue(MTemp)) {
			Temp.enqueue(MTemp);
			MString += to_string(MTemp.getID());
			MString += ", ";
		}

		while (Temp.dequeue(MTemp))
			WMMissions.enqueue(MTemp);

		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}

	output = EString + PString + MString;
	return output;
}

string MarsStation::IEMString()
{
	string output = "";
	string MString = "{";
	string PString = "(";
	string EString = "[";

	LinkedQueue<Mission> Temp;
	Mission MTemp;


	while (IEMissions.dequeue(MTemp)) {
		Temp.enqueue(MTemp);
		if (MTemp.getInfo().MissionType == EmergencyMission) {
			EString += to_string(MTemp.getID()) + "/" + to_string(MTemp.getRoverID());
			EString += ", ";
		} else if (MTemp.getInfo().MissionType == PolarMission) {
			PString += to_string(MTemp.getID()) + "/" + to_string(MTemp.getRoverID());
			PString += ", ";
		}
		else if (MTemp.getInfo().MissionType == MountainousMission) {
			MString += to_string(MTemp.getID()) + "/" + to_string(MTemp.getRoverID());
			MString += ", ";
		}
	}

	while (Temp.dequeue(MTemp)) IEMissions.enqueue(MTemp);

	if (EString == "[") EString = "";
	else {
		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}
	if (PString == "(") PString = "";
	else {
		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}
	if (MString == "{") MString = "";
	else {
		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}


	output = EString + PString + MString;
	return output;
}

string MarsStation::ARString()
{
	string output = "";
	string MString = "";
	string PString = "";
	string EString = "";

	LinkedQueue<Rover> Temp;
	Rover RTemp;

	if (!ERovers.isEmpty()) {
		EString = "[";
		while (ERovers.dequeue(RTemp)) {
			Temp.enqueue(RTemp);
			EString += to_string(RTemp.getID());
			EString += ", ";
		}

		while (Temp.dequeue(RTemp))
			ERovers.enqueue(RTemp);

		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}

	if (!PRovers.isEmpty()) {
		PString = "(";
		while (PRovers.dequeue(RTemp)) {
			Temp.enqueue(RTemp);
			PString += to_string(RTemp.getID());
			PString += ", ";
		}

		while (Temp.dequeue(RTemp))
			PRovers.enqueue(RTemp);

		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}

	if (!MRovers.isEmpty()) {
		MString = "{";
		while (MRovers.dequeue(RTemp)) {
			Temp.enqueue(RTemp);
			MString += to_string(RTemp.getID());
			MString += ", ";
		}

		while (Temp.dequeue(RTemp))
			MRovers.enqueue(RTemp);

		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}

	output = EString + PString + MString;
	return output;
}

string MarsStation::ICRString()
{
	string output = "";
	string MString = "{";
	string PString = "(";
	string EString = "[";

	LinkedQueue<Rover> Temp;
	Rover RTemp;


	while (ICRovers.dequeue(RTemp)) {
		Temp.enqueue(RTemp);
		if (RTemp.getInfo().Type == EmergencyRover) {
			EString += to_string(RTemp.getID());
			EString += ", ";
		}
		else if (RTemp.getInfo().Type == PolarRover) {
			PString += to_string(RTemp.getID());
			PString += ", ";
		}
		else if (RTemp.getInfo().Type == MountainousRover) {
			MString += to_string(RTemp.getID());
			MString += ", ";
		}
	}

	while (Temp.dequeue(RTemp)) ICRovers.enqueue(RTemp);

	if (EString == "[") EString = "";
	else {
		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}
	if (PString == "(") PString = "";
	else {
		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}
	if (MString == "{") MString = "";
	else {
		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}


	output = EString + PString + MString;
	return output;
}

string MarsStation::IMRString()
{
	string output = "";
	string MString = "{";
	string PString = "(";
	string EString = "[";

	LinkedQueue<Rover> Temp;
	Rover RTemp;


	while (IMRovers.dequeue(RTemp)) {
		Temp.enqueue(RTemp);
		if (RTemp.getInfo().Type == EmergencyRover) {
			EString += to_string(RTemp.getID());
			EString += ", ";
		}
		else if (RTemp.getInfo().Type == PolarRover) {
			PString += to_string(RTemp.getID());
			PString += ", ";
		}
		else if (RTemp.getInfo().Type == MountainousRover) {
			MString += to_string(RTemp.getID());
			MString += ", ";
		}
	}

	while (Temp.dequeue(RTemp)) IMRovers.enqueue(RTemp);

	if (EString == "[") EString = "";
	else {
		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}
	if (PString == "(") PString = "";
	else {
		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}
	if (MString == "{") MString = "";
	else {
		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}


	output = EString + PString + MString;
	return output;
}

string MarsStation::CMString()
{
	string output = "";
	string MString = "{";
	string PString = "(";
	string EString = "[";

	LinkedQueue<Mission> Temp;
	Mission MTemp;


	while (CMissions.dequeue(MTemp)) {
		Temp.enqueue(MTemp);
		if (MTemp.getInfo().MissionType == EmergencyMission) {
			EString += to_string(MTemp.getID());
			EString += ", ";
		}
		else if (MTemp.getInfo().MissionType == PolarMission) {
			PString += to_string(MTemp.getID());
			PString += ", ";
		}
		else if (MTemp.getInfo().MissionType == MountainousMission) {
			MString += to_string(MTemp.getID());
			MString += ", ";
		}
	}

	while (Temp.dequeue(MTemp)) CMissions.enqueue(MTemp);

	if (EString == "[") EString = "";
	else {
		EString.pop_back();
		EString.pop_back();
		EString += "] ";
	}
	if (PString == "(") PString = "";
	else {
		PString.pop_back();
		PString.pop_back();
		PString += ") ";
	}
	if (MString == "{") MString = "";
	else {
		MString.pop_back();
		MString.pop_back();
		MString += "} ";
	}


	output = EString + PString + MString;
	return output;
}

string MarsStation::FormatOutput()
{
	string output;
	output = "Current Day:" + to_string(getCurrentDay()) + "\n";
	// Waiting Missions
	output += to_string(WEMissions.count() + WMMissions.count() + WPMissions.count());
	output += " Waiting Missions: ";
	output += WMString();
	output += "\n";
	output += "------------------------------------------------------------------------\n";
	// In Execution Missions
	output += to_string(IEMissions.count());
	output += " In-Execution Missions/Rovers: ";
	output += IEMString();
	output += "\n";
	output += "------------------------------------------------------------------------\n";
	// Availabe Rovers
	output += to_string(ERovers.count() + MRovers.count() + PRovers.count());
	output += " Available Rovers: ";
	output += ARString();
	output += "\n";
	output += "------------------------------------------------------------------------\n";
	// In Checkup Rovers
	output += to_string(ICRovers.count());
	output += " In-Checkup Rovers: ";
	output += ICRString();
	output += "\n";
	output += "------------------------------------------------------------------------\n";
	// In Maintenance Rovers
	output += to_string(IMRovers.count());
	output += " In-Maintenance Rovers: ";
	output += IMRString();
	output += "\n";
	output += "------------------------------------------------------------------------\n";
	// Completed Missions
	output += to_string(CMissions.count());
	output += " Completed Missions: ";
	output += CMString();
	output += "\n";

	return output;
}

void MarsStation::CheckForMode()
{
	if (ModeType == Interactive) {
		UserInterface.printToScreen(FormatOutput());
		UserInterface.getUserInput();
	} else if (ModeType == StepByStep) {
		UserInterface.printToScreen(FormatOutput());
		Sleep(1000);
	} else
		return;
}

bool MarsStation::CheckForExit()
{
	if (Events.isEmpty() && WEMissions.isEmpty() && WMMissions.isEmpty() && WPMissions.isEmpty() && IEMissions.isEmpty() && ICRovers.isEmpty() && IMRovers.isEmpty())
		return true;

	return false;
}

void MarsStation::Operating() {
	ReadDataInput(file);

	if (exitFile) {
		UserInterface.printFileNotFound();
	}

	CheckForCorner(exit);
	bool exitLast = exit;

	if (exit &&!exitFile) {
		UserInterface.printRMmismatch();
	} else {
		UserInterface.getOutputFilename(outputFilename);
		UserInterface.getUserMode(ModeType);
	}

	while (!exit && !exitFile) {
		EventsOperating();

		MissionsOperating();

		RoversOperating();

		CheckForMode();

		if (CheckForExit()) {
			exit = true;
			break;
		}
		incCurrentDay();
	}

	if (!exitLast && !exitFile) {
		UserInterface.printLast();
	}
}

void MarsStation::CreateOutputFile()
{
	ofstream file;
	string output = "CD\tID\tFD\tWD\tED\n";
	int nMM = 0; int nPM = 0; int nEM = 0;
	int nMR = 0; int nPR = 0; int nER = 0;
	int sumWait = 0; int sumExec = 0;

	Rover RDelete;
	Mission MDelete;

	CMissions.reOrderCMissions();

	while (ERovers.dequeue(RDelete)) nER++;
	while (MRovers.dequeue(RDelete)) nMR++;
	while (PRovers.dequeue(RDelete)) nPR++;

	while (CMissions.dequeue(MDelete)) {
		if (MDelete.getInfo().MissionType == MountainousMission)
			nMM++;
		else if (MDelete.getInfo().MissionType == PolarMission)
			nPM++;
		else
			nEM++;

		sumWait += MDelete.getWD();
		sumExec += MDelete.getED();

		output += to_string(MDelete.getCD()) + "\t";
		output += to_string(MDelete.getID()) + "\t";
		output += to_string(MDelete.getFD()) + "\t";
		output += to_string(MDelete.getWD()) + "\t";
		output += to_string(MDelete.getED()) + "\n";
	}

	output += "..........................................\n";
	output += "..........................................\n";

	output += "Missions: " + to_string(nMM+nPM+nEM) + "\t";
	output += "[M: " + to_string(nMM) + ", P: " + to_string(nPM) + ", E: " + to_string(nEM) + "]\n";

	output += "Rovers: " + to_string(nMR + nPR + nER) + "\t";
	output += "[M: " + to_string(nMR) + ", P: " + to_string(nPR) + ", E: " + to_string(nER) + "]\n";

	output += "Avg Wait = " + to_string(((double)sumWait) / (nMM + nPM + nEM)) + ", ";
	output += "Avg Exec = " + to_string(((double)sumExec) / (nMM + nPM + nEM)) + "\n";

	output += "Auto-promoted: " + to_string(((double)nAutoP*100) / (nMM)) + "%\n";

	file.open(outputFilename);
	file.clear();
	file << output;
}