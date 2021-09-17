#include "Smartphone.h"

Smartphone::Smartphone()
{
	_goodsType = SMARTPHONE;
	cout << "Please input the name: ";
	cin >> _name;
	cout << "Please input the price in Euros: ";
	cin >> _price;
	cout << "Please input the color: ";
	cin >> _color;
	cout << "Please input the screen size in inches: ";
	cin >> _screenSize;
	cout << "Please input the battery capacity in mAh: ";
	cin >> _batteryLife;
}

Smartphone::~Smartphone()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class Smartphone was deleted" << endl;
#endif
}

string Smartphone::ShowSpec() const
{
	return getType() + ": " + _name + " P:" + to_string(_price) + "eur, C:" + _color + ", Bat:" + to_string(_batteryLife) + "mAh, Scr:" + to_string(_screenSize) + "inch";
}

