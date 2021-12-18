#include "ChatMessagesList.h"

ChatMessagesList::ChatMessagesList()
{
	loadFromBinaryFile();
}

ChatMessagesList::~ChatMessagesList()
{
	saveToBinaryFile();
}

size_t ChatMessagesList::getMessagesCount() const
{
	return _messagesList.size();
}

bool ChatMessagesList::isEmpty() const
{
	return _messagesList.empty();
}

void ChatMessagesList::addMessage(const ChatMessage& newMessage)
{
	_messagesList.insert(newMessage);
}

ChatMessage ChatMessagesList::findMessage(const std::string& receiver)
{
	ChatMessage tmpMsg("", "", receiver);
	messagesListType::const_iterator iterFound = _messagesList.find(tmpMsg);
	if (iterFound != std::end(_messagesList))
	{
		tmpMsg = *iterFound;
		_messagesList.erase(iterFound);
		return tmpMsg;
	}
	else
	{
		tmpMsg = ChatMessage();
		return tmpMsg;
	}
}

bool ChatMessagesList::saveToBinaryFile() const
{
	std::string fname = std::string(fileMessagesListPrefix) + std::string(fileMessagesListPostfix);
	std::ofstream messagesFile(fname, std::ios::binary | std::ios::trunc);
	
	if (messagesFile.is_open())
	{
		std::string tmpStr;			// Буфер для получения текстовых полей структуры ChatMessage
		char* tmpCharArr;			// Буфер для сохранения текстовых полей структуры ChatMessage
		size_t tmpSize;		// Счётчик записываемых значений

		tmpSize = _messagesList.size();
		messagesFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(unsigned int)); // Сохраняем число сообщений

		for (auto msgNum : _messagesList)
		{
			tmpStr = msgNum.getSender(); // Сохраняем логин отправителя
			tmpSize = tmpStr.length();
			tmpCharArr = new char[tmpSize + 1];
			strcpy_s(tmpCharArr, tmpSize + 1, tmpStr.c_str());
			messagesFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(unsigned int));
			messagesFile.write(reinterpret_cast<char*>(tmpCharArr), tmpSize);
			delete[] tmpCharArr;

			tmpStr = msgNum.getMessage(); // Сохраняем текст сообщения
			tmpSize = tmpStr.length();
			tmpCharArr = new char[tmpSize + 1];
			strcpy_s(tmpCharArr, tmpSize + 1, tmpStr.c_str());
			messagesFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(unsigned int));
			messagesFile.write(reinterpret_cast<char*>(tmpCharArr), tmpSize);
			delete[] tmpCharArr;

			tmpStr = msgNum.getReceiver(); // Сохраняем логин получателя
			tmpSize = tmpStr.length();
			tmpCharArr = new char[tmpSize + 1];
			strcpy_s(tmpCharArr, tmpSize + 1, tmpStr.c_str());
			messagesFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(unsigned int));
			messagesFile.write(reinterpret_cast<char*>(tmpCharArr), tmpSize);
			delete[] tmpCharArr;
		}
		messagesFile.close();
		return true;
	}
	else
		return false;
}

bool ChatMessagesList::loadFromBinaryFile()
{
	std::string fname = std::string(fileMessagesListPrefix) + std::string(fileMessagesListPostfix);
	std::ifstream messagesFile(fname, std::ios::binary);

	if (messagesFile.is_open())
	{
		char* tmpCharBuf;								// Буфер для чтения текстовых полей структуры ChatMessage
		std::string tmpStrS, tmpStrM, tmpStrR;			// Буфер для сохранения текстовых полей структуры ChatMessage
		unsigned int tmpSize;							// Счётчик считываемых значений

		if (!messagesFile.read(reinterpret_cast<char*>(&tmpSize), sizeof tmpSize))
			return false;
		unsigned int usersCount = tmpSize;	// Читаем число пользователей

		for (unsigned int userNum = 0; userNum < usersCount; ++userNum)
		{

			if (!messagesFile.read(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize)))
				return false;
			tmpCharBuf = new char[tmpSize + 1];
			if (!messagesFile.read(tmpCharBuf, tmpSize))
				return false;
			tmpCharBuf[tmpSize] = 0;
			tmpStrS = tmpCharBuf;			// Читаем логин отправителя
			delete[] tmpCharBuf;

			if (!messagesFile.read(reinterpret_cast<char*>(&tmpSize), sizeof tmpSize))
				return false;
			tmpCharBuf = new char[tmpSize + 1];
			if (!messagesFile.read(tmpCharBuf, tmpSize))
				return false;
			tmpCharBuf[tmpSize] = 0;
			tmpStrM = tmpCharBuf;			// Читаем текст сообщения
			delete[] tmpCharBuf;

			if (!messagesFile.read(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize)))
				return false;
			tmpCharBuf = new char[tmpSize + 1];
			if (!messagesFile.read(tmpCharBuf, tmpSize))
				return false;
			tmpCharBuf[tmpSize] = 0;
			tmpStrR = tmpCharBuf;			// Читаем логин получателя
			delete[] tmpCharBuf;

			ChatMessage tmpMsg(tmpStrS, tmpStrM, tmpStrR);
			addMessage(tmpMsg);

		}
		messagesFile.close();
		return true;
	}
	else
		return false;
}
