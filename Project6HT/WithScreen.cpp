#include "WithScreen.h"

WithScreen::~WithScreen()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class WithScreen was deleted" << endl;
#endif
}

unsigned int WithScreen::getScreenSize() const
{
	return _screenSize;
}

string WithScreen::ShowSpec() const
{
	return getType() + ": " + _name + " P:" + to_string(_price) + "eur, C:" + _color + ", Scr:" + to_string(_screenSize) + "inch";
}

