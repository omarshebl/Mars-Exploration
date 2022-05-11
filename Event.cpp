#include "Event.h"

Event::Event(int _ED, int _ID, int _type)
{
	ED = _ED;
	ID = _ID;
	type = _type;
}

int Event::getID() const
{
	return ID;
}

int Event::getED() const
{
	return ED;
}

int Event::gettype() const
{
	return type;
}
