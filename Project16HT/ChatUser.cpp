#include "ChatUser.h"

ChatUser::ChatUser(const std::string& login, const std::string& name, Block hash) : _login(login), _name(name), _passSha1Hash(hash), _online(false)
{
}

ChatUser::ChatUser(const std::string& login, const std::string& name, const std::string& password)
{
	_login = login;
	_name = name;
	_passSha1Hash = sha1(password.c_str(), password.length());
	_online = false;
}

ChatUser::ChatUser(const std::string& login) : _login(login), _name(""), _passSha1Hash(nullptr), _online(false)
{
}

ChatUser::~ChatUser()
{
}

std::string ChatUser::getLogin() const
{
	return _login;
}

std::string ChatUser::getName() const
{
	return _name;
}

std::string ChatUser::getHash() const
{
	if (_passSha1Hash == nullptr)
	{
		std::cout << "[FAILED] _passSha1Hash == nullptr!" << std::endl;
		return "";
	}

	unsigned char byteArray[sizeof(unsigned int)];
	std::string retVal;
	char buff[10];
	for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
	{
		intToByte(_passSha1Hash[i], byteArray);
		for (unsigned int j = 0; j < sizeof(unsigned int); ++j)
		{
			_snprintf_s(buff, sizeof(buff), "%02x:", (unsigned char)byteArray[j]);
			retVal = retVal + buff;
		}
	}
	return retVal;
}

Block ChatUser::getHashBinary() const
{
	/*
	if (_passSha1Hash == nullptr)
		return nullptr;
	Block retVal = new uint[SHA1HASHLENGTHUINTS];
	memcpy(retVal, _passSha1Hash, SHA1HASHLENGTHBYTES);
	return retVal;
	*/
	return _passSha1Hash;
}

void ChatUser::setHash(Block newHash)
{
	if (_passSha1Hash != nullptr)
		delete[] _passSha1Hash;
	_passSha1Hash = newHash;
}

bool ChatUser::compareHashes(Block other) const
{
    if ((other == nullptr) || (_passSha1Hash == nullptr))
        return false;
    for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
        if (other[i] != _passSha1Hash[i])
            return false;
    return true;
}

bool ChatUser::compareHashes(ChatUser other) const
{
    return compareHashes(other._passSha1Hash);
}

bool ChatUser::isOnline() const
{
	return _online;
}

void ChatUser::setOnline()
{
	_online = true;
}

void ChatUser::setOffline()
{
	_online = false;
}


void intToByte(unsigned int n, unsigned char* result)
{
	result[3] = n & 0x000000ff;
	result[2] = (n & 0x0000ff00) >> 8;
	result[1] = (n & 0x00ff0000) >> 16;
	result[0] = (n & 0xff000000) >> 24;
}