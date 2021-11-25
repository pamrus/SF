#pragma once

#include <string.h>
#include <iostream>
#include <functional>
#include "sha1.h"


/* Заготовка класса взята из задания 15.7.3
*  В оригинале имена фруктов и их количество хранятся в динамическом массиве пар ключ-значение.
*  Имена заменяются с char[] на string
*  Значения заменяются с int на int[]
*  Функция hash_func выполнена согласно ТЗ, 
*  за исключением добавки встроенного алгоритма std::hash перед выполнением хеширования умножением.
*/

class HashTable 
{
public:

    HashTable();
    ~HashTable();
    void add(std::string name, unsigned int* data);
    
    void del(std::string name);
    unsigned int* find(std::string name);

private:

    enum enPairStatus 
    {
        free,
        engaged,
        deleted
    };

    struct Pair // пара ключ-значение
    { 

        Pair() : _name(""), _status(enPairStatus::free) { }

        Pair(std::string name, unsigned int data[SHA1HASHLENGTHUINTS]) : _name(name), _status(enPairStatus::engaged) 
        { 
            for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
                _data[i] = data[i];
        }

        Pair& operator = (const Pair& other) 
        {   
            _name = other._name;
            _status = other._status;
            for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
                _data[i] = other._data[i];
            return *this;
        }

        /* bool operator == (const Pair& other) {
            return  status == other.status &&
                (status != enPairStatus::engaged || (_data == other._data && !strcmp(_name, other._name)));
        }  // TODO: исправить! 
        */ 

        std::string _name;
        unsigned int _data[SHA1HASHLENGTHUINTS]{ 0,0,0,0,0 };

        enPairStatus _status;
    };

    void resize();
    int hash_func(std::string name, int offset);

    Pair* array;
    unsigned int mem_size;
    unsigned int count;
};
