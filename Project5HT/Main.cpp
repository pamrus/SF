#include <iostream>
#include "VendingLib.h"
using namespace std;

// Тестовая неинтерактивная программа для проверки функционала созданных классов.

int main()
{
	setlocale(LC_ALL, "");

	VendingMachine* machine1 = new VendingMachine(5); // 5 карманов для слотов в машине
	SnackSlot* slot1 = new SnackSlot(10);
	SnackSlot* slot2 = new SnackSlot(10);
	SnackSlot* slot3 = new SnackSlot(100);
	SnackSlot* slot4 = new SnackSlot(5);
	Snack* boughtSnack;

	// Занимаем первый 10-местный слот первой группой снеков (положили 9 штук, но 2 позже достанем - останется 3 свободных места):
	Snack* bounty1 = new Snack("Bounty");
	Snack* snickers1 = new Snack("Snickers", 2, 200);
	Snack* snack3 = new Snack("Snack3", 3, 300);
	Snack* snack4 = new Snack("Snack4", 4, 400);
	Snack* snack5 = new Snack("Snack5", 5, 500);
	Snack* snack6 = new Snack("Snack6", 6, 600);
	Snack* snack7 = new Snack("Snack7", 7, 700);
	Snack* snack8 = new Snack("Snack8", 8, 800);
	Snack* snack9 = new Snack("Snack9", 9, 900);
	slot1->addSnack(bounty1);
	slot1->addSnack(snickers1);
	slot1->addSnack(snack3);
	slot1->addSnack(snack4);
	slot1->addSnack(snack5);
	slot1->addSnack(snack6);
	slot1->addSnack(snack7);
	slot1->addSnack(snack8);
	slot1->addSnack(snack9);
	machine1->addSlot(slot1);

	// Занимаем второй 10-местный слот второй группой снеков (2 штуки, остаётся 8 свободных мест):
	Snack* bounty2 = new Snack("Bounty");
	Snack* snickers2 = new Snack("Snickers");
	slot2->addSnack(bounty2);
	slot2->addSnack(snickers2);
	machine1->addSlot(slot2);

	// Создаём третий 100-местный слот (пустой, остаётся 100 свободных мест):
	machine1->addSlot(slot3);

	// Создаём четвёртый 5-местный слот (пустой, остаётся 5 свободных мест):
	machine1->addSlot(slot4);

	// Начинаем манипуляции со снеками и слотами
	machine1->getSlotsList(); // Выводим начальное распределение снеков по слотам перед покупками
	machine1->getSnacksListByIndex(1); // Выводим начальное распределение снеков в первом слоте перед покупками
	
	boughtSnack = machine1->giveSnack(1, 5); // Взяли один снек из первого слота, слот стал последним (4)
	delete boughtSnack;
	machine1->getSnacksListByIndex(4); // Выводим изменения

	boughtSnack = machine1->giveSnack(4, 7); // Взяли ещё один снек из того же слота
	delete boughtSnack;
	machine1->getSnacksListByIndex(4); // Выводим изменения

	boughtSnack = machine1->giveSnack(4, 9); // Пробуем взять несуществующий снек
	delete boughtSnack;
	machine1->extractSlot(5); // Пробуем извлечь несуществующий слот

	// Удаляем третий слот:
	machine1->extractSlot(2);
	machine1->getSlotsList(); // Выводим изменения


	// Итого имеем:
	// 2 незанятых слотами кармана в машине
	// 3+8+5 = 16 свободных мест для снеков
	// 1 установленный, но пустой слот
	// 7+2 снеков
	// Проверим:
	cout << "Free boxes: " << machine1->getEmptySlotsCount() << endl; // Незанятых карманов
	cout << "Total number of free cells: " << machine1->getEmptyCellsCount() << endl; // Всего свободных ячеек
	cout << "Number of empty slots: " << machine1->getFreeSlotsCount() << endl; // Полностью пустых слотов
	cout << "Total number of snacks: " << machine1->getFilledCellsCount() << endl; // Количество оставшихся продуктов

	delete machine1; // Деструктор потянет деструкторы остальных классов и куча очистится

	return 0;
}