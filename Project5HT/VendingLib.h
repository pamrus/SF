#pragma once
#include <iostream>
#include <string>
#define DEBUG

using namespace std;

class Snack
{
public:
	Snack(string name);
	Snack(string name, unsigned int price);
	Snack(string name, unsigned int price, unsigned int energy);
	~Snack();

	string getSnackName() const;
	unsigned int getSnackPrice() const;
	string getSnackInfo() const;

private:
	string name;
	unsigned int price;
	unsigned int energy;
};

class SnackSlot
{
	// Слот (Slot) для хранения определённого количества снеков
	// Ячейка (Cell) - место (вакантное или занятое) в слоте для хранения одного снека
public:
	SnackSlot(unsigned int cellsCount);
	~SnackSlot();

	bool addSnack(Snack* snack); // Добавить снек на свободное место
	Snack* giveSnack(unsigned int cellNumber); // Извлечь снек по номеру ячейки, сдвинуть остальные
	void getSnacksList() const; // Вывести таблицу с информацией о имеющихся в слоте снеках
	unsigned int getFreeCellsCount() const; // Вернуть число незанятых ячеек
	unsigned int getFilledCellsCount() const; // Вернуть число оставшихся продуктов
	void printFreeCellsCount() const; // Вывести число незанятых ячеек
	bool isEmpty() const; // Проверить, полностью ли пуст слот

private:
	unsigned int cellsCount; // Число ячеек (максимальное число снеков)
	Snack **snacksArray;
	unsigned int snacksCount; // Текущее число занятых ячеек
};

class VendingMachine
{
	// Машина (Machine), вмещающая определённое количество слотов
	// Карман (box) - место (вакантное или занятое) в машине для хранения одного слота
public:
	VendingMachine(unsigned int slotsCapacity);
	~VendingMachine();

	bool addSlot(SnackSlot* slot); // Добавить слот
	SnackSlot* extractSlot(unsigned int slotNumber); // Извлечь слот по номеру кармана, сдвинуть остальные
	void getSlotsList() const; // Вывести таблицу с информацией о имеющихся в слоте снеках
	void getSnacksListByIndex(unsigned int slotNumber) const; // Вывести таблицу с информацией о имеющихся в указанном слоте снеках
	Snack* giveSnack(unsigned int slotNumber, unsigned int cellNumber); // Извлечь слот, сдвинуть остальные, достать снек, задвинуть слот в конец
	unsigned int getFilledCellsCount() const; // Вернуть число оставшихся продуктов по всем слотам
//===== Не до конца понятна формулировка в ТЗ для функции getEmptySlotsCount
//===== ("Должно выводить количество пустых слотов для снеков"),
//===== поэтому создано три функции, похожих по описанию:
	unsigned int getEmptySlotsCount() const; // Вернуть число незанятых карманов
	unsigned int getEmptyCellsCount() const; // Вернуть число незанятых ячеек по всем установленным слотам
	unsigned int getFreeSlotsCount() const; // Вернуть число полностью пустых установленных слотов

private:
	unsigned int slotsCapacity; // Число карманов (максимальное число слотов)
	SnackSlot **slotsArray;
	unsigned int slotsCount; // Текущее число установленных слотов
};