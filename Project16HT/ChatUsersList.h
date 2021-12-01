#pragma once
#include <unordered_set>
#include <set>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "ChatUser.h"

constexpr char fileUserListPrefix[] = "DB_users";
constexpr char fileUserListPostfix[] = ".dat";

struct hashFunc { // Определяем способ вычисления хеш-функции для unordered_set только по логину пользователя.
	size_t operator()(const ChatUser& user) const {
		return hash<string>()(user.getLogin());
	}
};

/*inline bool operator==(ChatUser const& lhs, ChatUser const& rhs) { // Определяем оператор сравнения элементов unordered_set только по логину
	return (lhs.getLogin().compare(rhs.getLogin()) == 0);
}*/

struct equalFunc {
	bool operator()(const ChatUser& lhs, const ChatUser& rhs) const noexcept {
		return (lhs.getLogin().compare(rhs.getLogin()) == 0);
	}
};

using table_type = std::unordered_set<ChatUser, hashFunc, equalFunc>; // Определяем тип контейнера данных для списка пользователей

/*
* Класс ChatUsersList для хранения пользователей <ChatUser> в контейнере типа unordered_set.
* Поскольку логины уникальны, вычисления хеша в таблице производится по ним.
* 
*/
class ChatUsersList
{
public:
	ChatUsersList();
	~ChatUsersList();

	bool addUser(const ChatUser& newUser);					// Проверка пользователя по множеству запрещённых и добавление в контейнер
	
	bool login(const ChatUser& newUser);					// Проверка на правильность пароля по логину

	bool saveToBinaryFile() const;							// Сохранение базы в бинарный файл
	bool loadFromBinaryFile();								// Чтение базы из текстового файла

	bool isUserRegistered(std::string login);			

private:
	table_type _table;						// Хеш-таблица для хранения зарегистрированных пользователей
	std::set<std::string> _forbiddenLogins; // Множество для хранения запрещённых при регистрации значений логина - перевести на unordered_set?

	table_type::iterator findUser(const std::string& login);
};

