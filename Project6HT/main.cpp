#include <iostream>
#include <string>
#include "IElectronics.h"
#include "Portable.h"
#include "WithScreen.h"
#include "Player.h"
#include "Smartphone.h"
#include "TVSet.h"
#include "Store.h"
using namespace std;

/*
Иерархия наследования: 
IElectronics - Portable, WithScreen : промежуточные классы
IElectronics - Portable - Player : простая прямая цепочка
IElectronics - WithScreen - TVSet : простая прямая цепочка
IElectronics - Portable + WithScreen - Smartphone: ромбовидное наследование
*/

void insertSeparator()
{
	cout << "==================================================================" << endl;
	return;
}

int main()
{
	setlocale(LC_ALL, "");

	// Создаём склад вместимостью 20 товаров
	Store* store1 = new Store(20); // Можно задать маленькую вместимость для проверки корректности обработки такой ситуации

	// ========================================================================================= //
	// ==  Вначале - тестовый неинтерактивный код, чтобы не вводить много раз одно и то же,   == //
	// ==						затем - доступен интерактив.								  == //
	// ==  Лучше собирать в конфигурации Release, иначе выведется много отладочной информации == //
	// ========================================================================================= //

	// Создаём объекты "телевизор" класса "Товар с экраном" (цепочка наследования: IElectronics->WithScreen):
	IElectronics* tv1 = new TVSet("USSR_KVN-49", 250, "brown", 7);
	IElectronics* tv2 = new TVSet("Samsung_QE82Q77RAU", 5490, "white", 82);
	// Создаём объекты "плеер" класса "Плеер" (цепочка наследования: IElectronics->Portable->Player):
	IElectronics* pla1 = new Player("Sony_Walkman-2", 230, "gray", 2000, 15);

	// Создаём объекты "смартфон" класса "Смартфон "(цепочка наследования: IElectronics->WithScreen,Portable->Smartphone : ромбовидное):
	IElectronics* sph1 = new Smartphone("Nokia_3310", 200, "gray", 850, 1);
	IElectronics* sph2 = new Smartphone("Apple_iPhone-12ProMax", 1273, "gray", 3687, 7);
/*
	Доступные геттеры объектов:
	cout << ")" << tv1->getName() << endl;
	cout << ")" << tv1->getPrice() << endl;
	cout << ")" << tv1->getColor() << endl;
	cout << ")" << tv1->getScreenSize() << endl;
	cout << ")" << tv1->ShowSpec() << endl;
	cout << tv1 << endl;
	cout << ")" << sph1->getName() << endl;
	cout << ")" << sph1->getPrice() << endl;
	cout << ")" << sph1->getColor() << endl;
	cout << ")" << sph1->getBatteryLife() << endl;
	cout << ")" << sph1->getScreenSize() << endl;
	cout << ")" << sph1->ShowSpec() << endl;
	cout << sph1 << endl;
*/

	// Добавляем товары на склад:
	store1->addItem(tv1);
	store1->addItem(tv2);
	store1->addItem(pla1);
	store1->addItem(sph1);
	store1->addItem(sph2);
	insertSeparator();

	// Выводим начальное состояние склада перед покупками:
	store1->getItemsList();
	insertSeparator();

	// Распродажа -20% на всё!
	store1->makeDiscount(20);

	// Выводим обновлённую таблицу товаров:
	store1->getItemsList();
	insertSeparator();

	// Покупаем товар по названию:
	IElectronics* soldElectronics;
	soldElectronics = store1->sellItemByName("Samsung_QE82Q77RAU");
	delete soldElectronics;

	// Выводим изменения на складе:
	store1->getItemsList();
	insertSeparator();

	// Пробуем купить не существующий на складе товар:
	soldElectronics = store1->sellItemByName("Motorola_RAZR-V3");
	delete soldElectronics;
	insertSeparator();

	// Дополнительный полезный функционал:
	cout << "Number of goods on store: " << store1->getItemsCount() << endl; // Число занятых позиций
	cout << "Number of free places: " << store1->getFreePlacesCount() << endl; // Число незанятых позиций
	cout << "Store is empty?: " << ( (store1->isEmpty()) ? "Yes" : "No" ) << endl;
	cout << "Store is full?: " << ((store1->isFull()) ? "Yes" : "No") << endl;

	// ============================================ //
	// ==========  Теперь - интерактив:  ========== //
	// ============================================ //

	IElectronics* newDevice;
	char operation = 0;
	string nameToBuy = "";
	while (1)
	{
		cout << "Please, choose operation: enter A - to add a new device to the store, " << endl;
		cout << "\t\t\t  B - to buy some device, " << endl;
		cout << "\t\t\t  L - to list store contents, " << endl;
		cout << "\t\t\t  any other - to exit" << endl;
		cin >> operation;
		if ((operation != 'A') && (operation != 'B') && (operation != 'L'))
			break;

		if (operation == 'A')
		{
			unsigned int type = 0;
			cout << "Please, enter the type: 1 - Smartphone, 2 - Player, 3 - TVSet" << endl;
			cin >> type;
			switch (type)
			{
			case 1:
				newDevice = new Smartphone();
				break;
			case 2:
				newDevice = new Player();
				break;
			case 3:
				newDevice = new TVSet();
				break;
			default:
				cout << "Wrong type!" << endl;
				newDevice = nullptr;
			}
			if (newDevice != nullptr)
			{
				store1->addItem(newDevice);
				store1->getItemsList();
			}

		}
		if (operation == 'B')
		{
			cout << "Please, enter device name: ";
			cin >> nameToBuy;
			newDevice = store1->sellItemByName(nameToBuy);
			delete newDevice;
		}
		if (operation == 'L')
		{
			store1->getItemsList();
		}
	}

	// Деструктор склада потянет деструкторы остальных классов и куча очистится:
	delete store1;

	return 0;
}