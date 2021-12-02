#pragma once
#include <unordered_set>
#include <set>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "ChatUser.h"

//NOTE: Основной класс определён в конце файла

constexpr char fileUserListPrefix[] = "DB_users";
constexpr char fileUserListPostfix[] = ".dat";

struct usersListHashFunc { // Определяем способ вычисления хеш-функции для unordered_set только по логину пользователя.
	size_t operator()(const ChatUser& user) const {
		return hash<string>()(user.getLogin());
	}
};

struct usersListEqualFunc { // Определяем оператор сравнения элементов unordered_set только по логину пользователя.
	bool operator()(const ChatUser& lhs, const ChatUser& rhs) const noexcept {
		return (lhs.getLogin().compare(rhs.getLogin()) == 0);
	}
};

using usersListType = std::unordered_set<ChatUser, usersListHashFunc, usersListEqualFunc>; // Определяем тип контейнера данных для списка пользователей.

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

	bool isUserRegistered(std::string login) const;
	std::string getNameByLogin(std::string login) const;
	size_t getNumberOfUsers();
	friend ostream& operator<<(ostream& output, const ChatUsersList userList); // Потоковый вывод 

private:
	usersListType _usersList;						// Хеш-таблица для хранения зарегистрированных пользователей
	std::set<std::string> _forbiddenLogins;			// Множество для хранения запрещённых при регистрации значений логина - перевести на unordered_set?

	usersListType::const_iterator findUser(const std::string& login) const;
};

