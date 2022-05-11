#include "MarsStation.h"

int main() {
	MarsStation* Main = new MarsStation;
	Main->StartUP();
	Main->Operating();
	Main->CreateOutputFile();
	return 1;
}