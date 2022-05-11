#pragma once
#include "Definitions.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

class UI
{
public:

	void startupAnimation();
	void getFilename(string& filename);
	void getUserMode(int& mode);
	void getUserInput();
	void getOutputFilename(string& filename);
	void printToScreen(string input);
	void printLast();
	void printRMmismatch();
	void printFileNotFound();
};

