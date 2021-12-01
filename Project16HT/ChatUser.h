#pragma once
#include <iostream>
#include "sha1.h"

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
	bool compareHashes(ChatUser other) const;
	bool isOnline() const;
	void setOnline();
	void setOffline();

private:
	std::string _login;
	std::string _name;
	Block _passSha1Hash;
	bool _online;
};

void intToByte(unsigned int n, unsigned char* result);
