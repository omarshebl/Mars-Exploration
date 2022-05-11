#pragma once
#include "Definitions.h"

class Event
{
protected:
	int ED, ID;
	int type;
	
public:
	Event(int _ED, int _ID, int _type);
	virtual EventInfo Execute() = 0;
	int getID() const;
	int getED() const;
	int gettype() const;
};

