#include "IElectronics.h"

IElectronics::~IElectronics()
{
#ifdef _DEBUG
	cout << "[  OK  ] Item \"" << _name << "\" of class IElectronics was deleted" << endl;
#endif
}
// - Можно определить, как "IElectronics::~IElectronics() = default;", если код внутри не нужен


string IElectronics::getType() const
{
	switch (_goodsType)
	{
	case DEFAULT: return "DEFAULT";
	case PORTABLE: return "PORTABLE";
	case WITHSCREEN: return "WITHSCREEN";
	case SMARTPHONE: return "Smartphone";
	case PLAYER: return "Player";
	case TVSET: return "TVSet";
	default: return "UNKNOWN!";
	}
}

string IElectronics::getName() const
{
	return _name;
}

unsigned int IElectronics::getPrice() const
{
	return _price;
}

string IElectronics::getColor() const
{
	return _color;
}

string IElectronics::ShowSpec() const
{
	return _name + " P:" + to_string(_price) + "eur, C:" + _color;
}

void IElectronics::changePrice(unsigned int newPrice)
{
	_price = newPrice;
	return;
}

bool IElectronics::makeDiscount(unsigned int discount)
{
	if ((discount <= 100) && (discount >= 0))
	{
		_price -= (_price * discount) / 100;
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "[FAILED] Failed to make discount = " << discount << "% !" << endl;
#endif
		return false;
	}
}

ostream& operator<<(ostream& output, const IElectronics* iElectronics)
{
	output << iElectronics->ShowSpec();
	return output;
}

