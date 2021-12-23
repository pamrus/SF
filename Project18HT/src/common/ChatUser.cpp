#include "../../include/common/ChatUser.h"

ChatUser::ChatUser(const std::string& login, const std::string& name, Block hash) : _login(login), _name(name), _passSha1Hash(hash), _online(false)
{
}

ChatUser::ChatUser(const std::string& login, const std::string& name, const std::string& password) : _login(login), _name(name), _online(false)
{
	_passSha1Hash = sha1(password.c_str(), (uint)password.length());
}

ChatUser::ChatUser(const std::string& login) : _login(login), _name(""), _passSha1Hash(nullptr), _online(false)
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
    if ( !other || !_passSha1Hash )
        return false;
    for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
        if (other[i] != _passSha1Hash[i])
            return false;
    return true;
}

bool ChatUser::compareHashes(const ChatUser& other) const
{
    return compareHashes(other._passSha1Hash);
}

bool ChatUser::isOnline() const
{
	return _online;
}

void ChatUser::setOnline() const
{
	_online = true;
}

void ChatUser::setOffline() const
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