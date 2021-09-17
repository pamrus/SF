#include "Store.h"

Store::Store(unsigned int storeCapacity)
{
	_storeCapacity = storeCapacity;
	itemsArray = new IElectronics * [_storeCapacity];
	_itemsCount = 0;
}

Store::~Store()
{
	for (unsigned int i = 0; i < _itemsCount; i++)
	{
		delete itemsArray[i];
	}
#ifdef _DEBUG
	cout << "[  OK  ] Store is empty" << endl;
#endif
}

bool Store::addItem(IElectronics* item)
{
	if (_itemsCount < _storeCapacity)
	{
		itemsArray[_itemsCount] = item;
		++_itemsCount;
#ifdef _DEBUG
		cout << "[  OK  ] Added " << item << " to cell N" << _itemsCount << endl;
#endif
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "[FAILED] Failed to add " << item << " ! ItemsCount = " << _itemsCount << endl;
#endif
		return false;
	}
}

void Store::getItemsList() const
{
	cout << "Current store contents:" << endl;
	for (unsigned int i = 0; i < _itemsCount; i++)
	{
		cout << i + 1 << ") " << itemsArray[i] << endl;
	}
	return;
}

unsigned int Store::getItemsCount() const
{
	return _itemsCount;
}

unsigned int Store::getFreePlacesCount() const
{
	return _storeCapacity - _itemsCount;
}

bool Store::isEmpty() const
{
	if(_itemsCount ==0 )
		return true;
	else
		return false;
}

bool Store::isFull() const
{
	if (_itemsCount == _storeCapacity)
		return true;
	else
		return false;
}

unsigned int Store::findFirstItemByName(const string itemName) const
{
	cout << "Searching for an item \"" << itemName << "..." << endl;
	for (unsigned int i = 0; i < _itemsCount; i++)
	{
		if (itemsArray[i]->getName() == itemName)
		{
			cout << "Found : " << i + 1 << ") " << itemsArray[i] << endl;
			return (i + 1);
		}
	}
	return 0;
}

IElectronics* Store::sellItemByIndex(unsigned int itemIndex)
{
	IElectronics* retVal;
	if ( ( itemIndex <= getItemsCount() ) && (itemIndex > 0))
	{
		retVal = itemsArray[itemIndex - 1];
		cout << "Please take " << retVal->getName() <<
			" for " << retVal->getPrice() << " Euros." << endl;
		for (unsigned int i = itemIndex - 1; i < _itemsCount - 1; ++i)
		{
			itemsArray[i] = itemsArray[i + 1];
		}
		--_itemsCount;
		return retVal;
	}
	else
	{
		cout << "Please choose another item." << endl;
#ifdef _DEBUG
		cout << "[FAILED] Failed to give item N " << itemIndex << " ! ItemsCount = " << _itemsCount << endl;
#endif
		return nullptr;
	}

}

IElectronics* Store::sellItemByName(const string itemName)
{
	unsigned int itemIndex = 0;
	itemIndex = findFirstItemByName(itemName);
	return sellItemByIndex(itemIndex);
}

bool Store::makeDiscount(unsigned int discount)
{
	if ((discount <= 100) && (discount >= 0))
	{
		cout << "Congratulations! " << discount << "% SALE!!!" << endl;
		for (unsigned int i = 0; i < _itemsCount; i++)
		{
			itemsArray[i]->makeDiscount(discount);
		}
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "[FAILED] Failed to make a sale, discount: " << discount << "% !" << endl;
#endif
		return false;
	}
}

