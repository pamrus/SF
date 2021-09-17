#include "TVSet.h"

TVSet::TVSet()
{
	_goodsType = TVSET;
	cout << "Please input the name: ";
	cin >> _name;
	cout << "Please input the price in Euros: ";
	cin >> _price;
	cout << "Please input the color: ";
	cin >> _color;
	cout << "Please input the screensize: ";
	cin >> _screenSize;
}

TVSet::~TVSet()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class TVSet was deleted" << endl;
#endif
}
