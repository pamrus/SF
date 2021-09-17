#include "Portable.h"

Portable::~Portable()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class Portable was deleted" << endl;
#endif
}

unsigned int Portable::getBatteryLife() const
{
	return _batteryLife;
}

string Portable::ShowSpec() const
{
	return _name + " P:" + to_string(_price) + "eur, C:" + _color + ", Bat:" + to_string(_batteryLife) + "mAh";
}

