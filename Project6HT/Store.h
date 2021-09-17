#pragma once
#include "IElectronics.h"

// ================================================ //
// ==  Класс склада товаров класса IElectronics  == //
// ================================================ //

class Store
{
public:
	Store() = default;												// Мало ли, наследовать придётся
	Store(unsigned int storeCapacity);								// Основной конструктор
	virtual ~Store();												// Опять же, на случай наследования

	bool addItem(IElectronics* item);								// Добавить товар
	void getItemsList() const;										// Вывести таблицу с информацией о имеющемся товаре
	unsigned int getItemsCount() const;								// Вернуть текущее количество товара
	unsigned int getFreePlacesCount() const;						// Вернуть количество свободных мест
	bool isEmpty() const;											// Вернуть факт полной пустоты
	bool isFull() const;											// Вернуть факт окончательной заполненности
	unsigned int findFirstItemByName(const string itemName) const;	// Вернуть позицию товара по названию (0 - если товар не найден)
	IElectronics* sellItemByIndex(unsigned int itemIndex);			// Вернуть товар по индексу, массив указателей сдвигается, количество уменьшается
	IElectronics* sellItemByName(const string itemName);			// Вернуть товар по названию
	bool makeDiscount(unsigned int discount);						// Распродажа всех товаров: скидка -discount%

protected:
	unsigned int _storeCapacity;									// Максимальная вместимость склада
	unsigned int _itemsCount;										// Текущее количество товара на складе
	IElectronics **itemsArray;										// Массив указателей на товар

};

