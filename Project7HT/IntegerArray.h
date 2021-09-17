#pragma once
#include <iostream>
#include "Bad_length.h"
#include "Bad_range.h"

class IntegerArray
{
public:
    IntegerArray() = default;
    IntegerArray(int length);
    IntegerArray(const IntegerArray& parentIntegerArray); // NEW: Конструктор копирования
    ~IntegerArray();

    void erase();
    int& operator[](int index);
    void reallocate(int newLength);
    void resize(int newLength);
    void insertBefore(int value, int index);
    void remove(int index);
    void insertAtBeginning(int value) { insertBefore(value, 0); }
    void insertAtEnd(int value) { insertBefore(value, m_length); }
    int findFirstElement(int value); // NEW: Индекс первого найденного значения, -1 - если такого элемента нет
    int getLength() const { return m_length; }
    void printContents(); // NEW: просто вывод на консоль

private:
    int m_length{};
    int* m_data{};

};