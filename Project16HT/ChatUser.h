#pragma once
#include <iostream>
#include "sha1.h"

/*
* Класс ChatUser для хранения информации о пользователе.
* 
*/
class ChatUser
{
public:
	ChatUser(const std::string& login, const std::string& name, Block hash);
	ChatUser(const std::string& login, const std::string& name, const std::string& password);
	ChatUser(const std::string& login);
	~ChatUser();

	std::string getLogin() const;
	std::string getName() const;
	std::string getHash() const;
	Block getHashBinary() const;
	void setHash(Block newHash);
	bool compareHashes(Block other) const;
	bool compareHashes(const ChatUser& other) const;
	bool isOnline() const;
	void setOnline() const;
	void setOffline() const;

private:
	std::string _login;
	std::string _name;
	Block _passSha1Hash;
	mutable bool _online;
};

void intToByte(unsigned int n, unsigned char* result);
