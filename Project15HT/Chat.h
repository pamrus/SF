#pragma once

#include <string>
#include <iostream>
#include "sha1.h"
#include "HashTable.h"


/* Заготовка класса взята из задания 15.4.2
*  В оригинале логины и SHA-1-хеши паролей хранятся в динамическом массиве, поиск пользователя в базе за O(n).
*  Хранилище заменяется на хеш-таблицу, char*ы заменяются на string.
*/ 
class Chat 
{
public:
    Chat();
    void reg(std::string login, std::string pass);
    bool login(std::string login, std::string _pass);

private:

    HashTable _data;

    int data_count;
};
