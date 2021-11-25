#include "Chat.h"

Chat::Chat() 
{
    data_count = 0;
}

void Chat::reg(std::string login, std::string pass)
{
    uint* pwHash = sha1(pass.c_str(), pass.length());
    _data.add(login, pwHash);
}

bool Chat::login(std::string login, std::string pass)
{
    unsigned int* foundHash = _data.find(login);
    if (foundHash == nullptr)
        return false;

    uint* inputHash = sha1(pass.c_str(), pass.length());
    return compareHashes(inputHash, foundHash);
}