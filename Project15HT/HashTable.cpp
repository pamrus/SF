#include "HashTable.h"

HashTable::HashTable() {
    count = 0;
    mem_size = 8;
    array = new Pair[mem_size];
}
HashTable::~HashTable() {
    delete[] array;
}

int HashTable::hash_func(std::string name, int offset) 
{
    
    std::size_t hashVal = std::hash<std::string>{}(name); // Считаем хеш логина (по модификации Fowler–Noll–Vo для MSVSC) 

    const double A = 0.7;
    const int M = 10;
    int prod = int(M * (A * hashVal - int(A * hashVal))); // Хеширование умножением

    return (prod % mem_size + offset * offset) % mem_size; // квадратичные пробы

}

void HashTable::del(std::string name)
{
    // Ваш код
    std::string searchName = name;
    // берем пробы по всем i от 0 до размера массива
    for (unsigned int i = 0; i < mem_size; i++) {
        int index = hash_func(name, i);

        if (array[index]._status == enPairStatus::free)
            return;

        if (array[index]._status == enPairStatus::engaged)
            if (!searchName.compare(array[index]._name))
            {
                array[index]._status = deleted;
                --count;
                return;
            }
    }

    return;
}

unsigned int* HashTable::find(std::string name) {
    // вернуть нужно значение, соответствующее ключу name
    // если такого ключа нету в таблице, то верните -1
    // Ваш код

    std::string searchName = name;
    // берем пробы по всем i от 0 до размера массива
    for (unsigned int i = 0; i < mem_size; i++) {
        int index = hash_func(name, i);

        if (array[index]._status == enPairStatus::free)
            return 0;
        if (array[index]._status != enPairStatus::deleted)
            if (!searchName.compare(array[index]._name))
                return array[index]._data;
    }

    return 0;
}


void HashTable::resize() {
    // Не забудьте добавить вызов resize() в нужное место
    // внутри метода add, а затем добавить элемент в увеличенный массив
    // Ваш код

    Pair* save_array = array; // запоминаем старый массив
    int oldSize = mem_size;

    mem_size *= 2;  // увеличиваем размер в два раза  
    count = 0; // обнуляем количество элементов
    array = new Pair[mem_size]; // выделяем новую память
    /*
    for (int i = 0; i < mem_size; i++) {
        array[i] = nullptr;
    }
    */

    for (int i = 0; i < oldSize; i++) {
        Pair current = save_array[i];
        add(current._name, current._data);
    }
    // чистим за собой
    delete[] save_array;
}

void HashTable::add(std::string name, unsigned int* data) {
    /* В этом методе нужно добавить вызов resize() */

    if (count >= mem_size)
        resize();


    int index = -1;
    unsigned int i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(name, i);
        if (array[index]._status == enPairStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }

    if (i >= mem_size) return; // все перебрали, нет места

    // кладем в свободную ячейку пару
    array[index] = Pair(name, data);
    count++;
}
