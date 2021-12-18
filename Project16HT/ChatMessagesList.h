#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include "ChatMessage.h"

//NOTE: Основной класс определён в конце файла

constexpr char fileMessagesListPrefix[] = "DB_messages";
constexpr char fileMessagesListPostfix[] = ".dat";

struct messagesListHashFunc { // Определяем способ вычисления хеш-функции для unordered_set только по логину получателя.
	size_t operator()(const ChatMessage& message) const {
		return hash<string>()(message.getReceiver());
	}
};

struct messagesListEqualFunc { // Определяем оператор сравнения элементов unordered_set только по логину получателя.
	bool operator()(const ChatMessage& lhs, const ChatMessage& rhs) const noexcept {
		return (lhs.getReceiver().compare(rhs.getReceiver()) == 0);
	}
};

using messagesListType = std::unordered_multiset<ChatMessage, messagesListHashFunc, messagesListEqualFunc>; // Определяем тип контейнера данных для списка сообщений.

/*
* Класс ChatMessagesList для хранения сообщений <ChatMessage> в контейнере типа unordered_multiset.
* Ключ - логин получателя.
* 
*/
class ChatMessagesList
{
public:
	ChatMessagesList();
	~ChatMessagesList();

	size_t getMessagesCount() const;
	bool isEmpty() const;
	void addMessage(const ChatMessage& newMessage);
	ChatMessage findMessage(const std::string& receiver);
	bool saveToBinaryFile() const;							// Сохранение базы в бинарный файл
	bool loadFromBinaryFile();								// Чтение базы из текстового файла

private:
	messagesListType _messagesList; // unordered_multiset, т.к. дубликаты по кешу допустимы (одинаковый получатель)
};
