#include "../../include/common/ChatMessage.h"

std::string ChatMessage::getMessage() const
{
	return _message;
}

std::string ChatMessage::getSender() const
{
	return _sender;
}

std::string ChatMessage::getReceiver() const
{
	return _receiver;
}

bool ChatMessage::isRead() const
{
	return _isRead;
}

std::string ChatMessage::getInfo() const
{
    std::string retVal = "";
    retVal = retVal + "Message: " + _sender + "->" + _receiver + " '" + _message + "'";
    return retVal;
}

std::ostream& operator<<(std::ostream& output, const ChatMessage& chatMessage)
{
	output << chatMessage.getInfo();
	return output;
}
