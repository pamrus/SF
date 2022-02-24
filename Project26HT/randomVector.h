#pragma once

#include <vector>
#include <random>
#include <functional> // bind
#include <numeric> // accumulate


#include <iostream>
#include <string> 
#include "util.h"


// Класс для хранения нормально-распределённых вещественных чисел.
// Запрещено хранение произвольных данных: экземпляр заполняется либо в конструкторе, ->
//		либо при отделении от другого экземпляра (который, по-определению, уже содержит правильные значения).
// Доступны read-only методы базового класса, подсчёт суммы всех элементов и отделение участка в конце вектора.
class randomVector : private std::vector<double>
{
public:
	randomVector();
	randomVector(const randomVector::iterator& start, const randomVector::iterator& stop, const randomVector& parent);
	randomVector(size_t length, double mean, double deviation);	// Генерируем вектор нормально-распределённых вещественных значений.

	void operator() (const std::string& prefix);	// Считаем сумму всех элементов

	double getMean() const;
	double getDeviation() const;
	double getGenerationDuration() const;
	double getAdditionDuration() const;
	double getTotalSum() const;

	randomVector cutoffTail(size_t length);	// Отделяем конец вектора в новый вектор
	
	// Делаем доступными некоторые методы и операторы базового класса:
	using vector<double>::size;
	using vector<double>::empty;
	double at(size_t index) const;	// Запись запрещена
	double operator[] (size_t index) const;	// Запись запрещена
	using vector<double>::clear;

	double _totalSum;

private:
	double _mean, _deviation;

	std::chrono::high_resolution_clock::time_point _generationStart, _additionStart;
	double _generationDuration, _additionDuration;

};

