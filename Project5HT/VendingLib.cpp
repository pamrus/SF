#include "VendingLib.h"

Snack::Snack(string name) // !Free cookies!
{
	this->name = name;
	this->price = 0;
	this->energy = 0;
}

Snack::Snack(string name, unsigned int price)
{
	this->name = name;
	this->price = price;
	this->energy = 0;
}

Snack::Snack(string name, unsigned int price, unsigned int energy)
{
	this->name = name;
	this->price = price;
	this->energy = energy;
}

Snack::~Snack()
{
#ifdef _DEBUG
	cout << "Snack \"" << name << "\" was deleted" << endl;
#endif
}

string Snack::getSnackName() const
{
	return name;
}

unsigned int Snack::getSnackPrice() const
{
	return price;
}

string Snack::getSnackInfo() const
{
	return name + " " + to_string(price) + " " + to_string(energy);
}


SnackSlot::SnackSlot(unsigned int cellsCount)
{
	this->cellsCount = cellsCount;
	snacksArray = new Snack * [this->cellsCount];
	snacksCount = 0;
}

SnackSlot::~SnackSlot()
{
	for (unsigned int i = 0; i < snacksCount; i++)
	{
		delete snacksArray[i];
	}
#ifdef _DEBUG
	cout << "Snackslot was deleted" << endl;
#endif
}

bool SnackSlot::addSnack(Snack *snack)
{
	if (snacksCount < cellsCount)
	{
		snacksArray[snacksCount] = snack;
		++snacksCount;
#ifdef _DEBUG
		cout << "Added " << snack->getSnackInfo() << " to cell N" << snacksCount << endl;
#endif
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "ERROR: " << "Did not add " << snack->getSnackInfo() << " ! SnacksCount = " << snacksCount << endl;
#endif
		return false;
	}
}

Snack* SnackSlot::giveSnack(unsigned int cellNumber)
{
	cout << "Searching for a snack in cell N" << cellNumber << "..." << endl;
	Snack* retVal;
	if ( (cellNumber <= snacksCount)&&(cellNumber != 0) )
	{
		retVal = snacksArray[cellNumber - 1];
		cout << "Please take " << snacksArray[cellNumber - 1]->getSnackName() <<
			" for " << snacksArray[cellNumber - 1]->getSnackPrice() << " Euros." << endl;
		for (unsigned int i = cellNumber - 1; i < snacksCount - 1; ++i)
		{
			snacksArray[i] = snacksArray[i + 1];
		}
		--snacksCount;
		return retVal;
	}
	else
	{
		cout << "Please choose another cell." << endl;
#ifdef _DEBUG
		cout << "ERROR: " << "Can not give snack from cell N " << cellNumber << " ! SnacksCount = " << snacksCount << endl;
#endif
		return nullptr;
	}
}

void SnackSlot::getSnacksList() const
{
	cout << "Current slot contents:" << endl;
	for (unsigned int i = 0; i < snacksCount; i++)
	{
		cout << i+1 << ") " << snacksArray[i]->getSnackInfo() << endl;
	}
}

unsigned int SnackSlot::getFreeCellsCount() const
{
	return cellsCount - snacksCount;
}

unsigned int SnackSlot::getFilledCellsCount() const
{
	return snacksCount;
}

void SnackSlot::printFreeCellsCount() const
{
	cout << "Number of free cells: " << getFreeCellsCount() << endl;
}

bool SnackSlot::isEmpty()const
{
	return (snacksCount == 0);
}

VendingMachine::VendingMachine(unsigned int slotsCapacity)
{
	this->slotsCapacity = slotsCapacity;
	slotsArray = new SnackSlot * [this->slotsCapacity];
	slotsCount = 0;
}

VendingMachine::~VendingMachine()
{
	for (unsigned int i = 0; i < slotsCount; i++)
	{
		delete slotsArray[i];
	}
#ifdef _DEBUG
	cout << "Vending machine was deleted" << endl;
#endif
}

bool VendingMachine::addSlot(SnackSlot* snackSlot)
{
	if (slotsCount < slotsCapacity)
	{
		slotsArray[slotsCount] = snackSlot;
		++slotsCount;
#ifdef _DEBUG
		cout << "Added slot to position N " << slotsCount << endl;
#endif
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "ERROR: " << "Did not add slot ! SlotsCount = " << slotsCount << endl;
#endif
		return false;
	}
}

SnackSlot* VendingMachine::extractSlot(unsigned int slotNumber)
{
	SnackSlot* retVal;
	cout << "Trying to extract slot from position N" << slotNumber << "...";
	if ( (slotNumber <= slotsCount)&&(slotNumber != 0) )
	{
		retVal = slotsArray[slotNumber - 1];
		for (unsigned int i = slotNumber - 1; i < slotsCount - 1; ++i)
		{
			slotsArray[i] = slotsArray[i + 1];
		}
		--slotsCount;
		cout << "OK!" << endl;
		return retVal;
	}
	else
	{
		cout << "FAILED!" << endl;
#ifdef _DEBUG
		cout << "ERROR: " << "Can not give extract slot from position N " << slotNumber << " ! SlotsCount = " << slotsCount << endl;
#endif
		return nullptr;
	}
}

void VendingMachine::getSlotsList() const
{
	cout << "Current slots in the machine:" << endl;
	for (unsigned int i = 0; i < slotsCount; i++)
	{
		cout << i + 1 << ") " << slotsArray[i]->getFilledCellsCount() << "-filled, " << slotsArray[i]->getFreeCellsCount() << "-free" << endl;
	}
}

void VendingMachine::getSnacksListByIndex(unsigned int slotNumber) const
{
	slotsArray[slotNumber - 1]->getSnacksList();
}

Snack* VendingMachine::giveSnack(unsigned int slotNumber, unsigned int cellNumber)
{
	SnackSlot* snackSlot = extractSlot(slotNumber);
	if (snackSlot != nullptr)
	{
		addSlot(snackSlot);
		return snackSlot->giveSnack(cellNumber);
	}
	else
		return nullptr;
}

unsigned int VendingMachine::getFilledCellsCount() const
{
	unsigned int retVal = 0;

	for (unsigned int i = 0; i < slotsCount; ++i)
	{
		retVal += slotsArray[i]->getFilledCellsCount();
	}

	return retVal;
}

unsigned int VendingMachine::getEmptySlotsCount() const
{
	return slotsCapacity - slotsCount;
}

unsigned int VendingMachine::getEmptyCellsCount() const
{
	unsigned int retVal = 0;

	for (unsigned int i = 0; i < slotsCount; ++i)
	{
		retVal += slotsArray[i]->getFreeCellsCount();
	}
	
	return retVal;
}

unsigned int VendingMachine::getFreeSlotsCount() const
{
	unsigned int retVal = 0;

	for (unsigned int i = 0; i < slotsCount; ++i)
	{
		if (slotsArray[i]->isEmpty())
			retVal++;
	}

	return retVal;
}
