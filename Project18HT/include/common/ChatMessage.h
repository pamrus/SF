#pragma once
#include <iostream>
#include "ChatUser.h"

/*
* Класс ChatMessage для хранения сообщения.
*
*/
class ChatMessage
{
public:
	ChatMessage(std::string sender, std::string message, std::string receiver) : _sender(sender), _receiver(receiver), _message(message), _isRead(false) {};
	ChatMessage() : _isRead(false) { };

	std::string getMessage() const;
	std::string getSender() const;
	std::string getReceiver() const;
	bool isRead() const;	// Пока не используется
	std::string getInfo() const;
	friend std::ostream& operator<<(std::ostream& output, const ChatMessage chatMessage);

private:
	std::string _message;
	std::string _sender;
	std::string _receiver;
	mutable bool _isRead; // Пока не используется
};
