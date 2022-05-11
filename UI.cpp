#include "UI.h"
using namespace std;
#include <windows.h>
void UI::startupAnimation() {
	cout << "Mars Station Booting UP";
	for (int i = 0; i < 10; i++) {
		Sleep(300);
		cout << ".";
	}
	cout << "." << flush;
	system("CLS");
}

void UI::getFilename(string& filename) {
	cout << "Please enter the input filename (including extension) >>  ";
	cin >> filename;
	system("CLS");
}

void UI::getUserMode(int& mode)
{
	cout << "Which mode would you like the data to be displayed in ?" << endl;
	cout << "1- Interactive Mode" << endl;
	cout << "2- Step-By-Step Mode" << endl;
	cout << "3- Silent Mode" << endl;
	cout << "  >>  ";
	cin >> mode;
	while (mode != 1 && mode != 2 && mode != 3) {
		cout << endl << " (1, 2 or 3) >>  ";
		cin >> mode;
	}
	system("CLS");
	if (mode == Interactive) {
		cout << "Interactive Mode" << endl << "Simulation Starts..." << endl << endl;
	} else if (mode == StepByStep) {
		cout << "Step-By-Step Mode" << endl << "Simulation Starts..." << endl << endl;
	} else if (mode == Silent) {
		cout << "Silent Mode" << endl << "Simulation Starts..." << endl;
	}
}

void UI::getUserInput()
{
	bool exit = false;
	while (!exit) {
		if (GetKeyState(VK_SPACE) & 0x8000) {
			Sleep(250);
			exit = true;
			break;
		}
	}
}

void UI::getOutputFilename(string& filename)
{
	cout << "Please enter output filename to save by (including extension) >>  ";
	cin >> filename;
	system("CLS");
}

void UI::printToScreen(string input)
{
	cout << input << endl;
}

void UI::printLast()
{
	cout << "Simulation ends, Output file created" << endl;
}

void UI::printRMmismatch()
{
	cout << "There was an error due to Rover & Mission mismatch, Please try to fix Rover quantities !" << endl;
	cout << "Simulation Ended, Output File NOT CREATED !" << endl;
}

void UI::printFileNotFound()
{
	cout << "File was NOT FOUND, Please try to enter right filename !" << endl;
	cout << "Simulation Ended, Output File NOT CREATED !" << endl;
}


