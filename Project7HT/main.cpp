#include <iostream>
#include "IntegerArray.h"
using namespace std;

// ===================================================================================== //
// ==  Тестовая неинтерактивная программа для проверки функционала созданных классов  == //
// ===================================================================================== //

int main()
{
	setlocale(LC_ALL, "");

    try
    {
        IntegerArray array(10);                             // !!! Доступна проверка правильности требуемой длины массива: -1

        // Fill the array with numbers 1 through 10
        for (int i{ 0 }; i < 10; ++i)                       // !!! Доступна проверка исключения для индекса за пределами размера массива: i < 11
            array[i] = i + 1;

        // Resize the array to 8 elements
        array.resize(8);                                    // !!! Доступна проверка исключения для отрицательных значений размера: -8

        // Insert the number 20 before element with index 5
        array.insertBefore(20, 5);                          // !!! Доступна проверка исключения для индекса за пределами размера массива: (20, 9)

        // Remove the element with index 3
        array.remove(8);                                    // !!! Доступна проверка исключения для индекса за пределами размера массива: 9

        // Add 30 and 40 to the end and beginning
        array.insertAtEnd(30);
        array.insertAtBeginning(40);

        // Print out all the numbers
        array.printContents();

        // NEW: Поиск первого появления в массиве элемента с заданным значением int IntegerArray::findFirstElement(int)
        int value = 20;
        cout << "Searching for element X = " << value << "... ";
        unsigned int index = array.findFirstElement(value);
        if (index == -1)
            cout << "Not found!" << endl;
        else
            cout << "Found Array[" << index << "] = " << array[index] << endl;

        // NEW: Конструктор копирования IntegerArray(const IntegerArray&)
        IntegerArray array2(array);
        // Проверяем, как скопировалось
        array2.printContents();

    }
    catch (exception& ex) // Перехватываем исключения, которые могли быть вызваны в конструкторах, [], insertBefore, remove нашими throw или в new.
    {
        cout << ex.what() << endl; // Все они наследуются от базового класса, так что программа не вылетит.
    }

    return 0;
}