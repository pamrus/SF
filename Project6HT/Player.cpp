#include "Player.h"

Player::Player()
{
	_goodsType = PLAYER;
	cout << "Please input the name: ";
	cin >> _name;
	cout << "Please input the price in Euros: ";
	cin >> _price;
	cout << "Please input the color: ";
	cin >> _color;
	cout << "Please input the battery capacity in mAh: ";
	cin >> _batteryLife;
	cout << "Please input the number of tracks: ";
	cin >> _totalTracks;
}

Player::~Player()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class Player was deleted" << endl;
#endif
}

unsigned int Player::ShowTotalTracks() const
{
	return _totalTracks;
}

string Player::ShowSpec() const
{
	return getType() + ": " + _name + " P:" + to_string(_price) + "eur, C:" + _color + ", Bat:" + to_string(_batteryLife) + "mAh, Tracks:" + to_string(_totalTracks);
}

