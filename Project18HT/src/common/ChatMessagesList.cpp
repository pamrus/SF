#include "../../include/common/ChatMessagesList.h"

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
		size_t tmpSize;				// Счётчик записываемых значений
		char* tmpCharSize = reinterpret_cast<char*>(&tmpSize);

		tmpSize = _messagesList.size();
		messagesFile.write(tmpCharSize, sizeof tmpSize); // Сохраняем число сообщений

		for (auto &msgNum : _messagesList)
		{
			tmpStr = msgNum.getSender(); // Сохраняем логин отправителя
			tmpSize = tmpStr.length();
			messagesFile.write(tmpCharSize, sizeof tmpSize);
			messagesFile.write(tmpStr.data(), tmpSize);

			tmpStr = msgNum.getMessage(); // Сохраняем текст сообщения
			tmpSize = tmpStr.length();
			messagesFile.write(tmpCharSize, sizeof tmpSize);
			messagesFile.write(tmpStr.data(), tmpSize);

			tmpStr = msgNum.getReceiver(); // Сохраняем логин получателя
			tmpSize = tmpStr.length();
			messagesFile.write(tmpCharSize, sizeof tmpSize);
			messagesFile.write(tmpStr.data(), tmpSize);
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
		std::unique_ptr<char[]> tmpCharBuf;				// Буфер для чтения текстовых величин
		std::string tmpStrS, tmpStrM, tmpStrR;			// Буфер для сохранения текстовых полей структуры ChatMessage
		size_t tmpSize;									// Счётчик считываемых значений
		char* tmpCharSize = reinterpret_cast<char*>(&tmpSize);

		if (!messagesFile.read(tmpCharSize, sizeof tmpSize))
			return false;
		size_t usersCount = tmpSize;	// Читаем число пользователей

		for (size_t userNum = 0; userNum < usersCount; ++userNum)
		{

			if (!messagesFile.read(tmpCharSize, sizeof tmpSize))
				return false;
			tmpCharBuf.reset(new char[tmpSize + 1]);
			if (!messagesFile.read(tmpCharBuf.get(), tmpSize))
				return false;
			tmpCharBuf[tmpSize] = '\0';
			tmpStrS = std::string(tmpCharBuf.get());			// Читаем логин отправителя


			if (!messagesFile.read(tmpCharSize, sizeof tmpSize))
				return false;
			tmpCharBuf.reset(new char[tmpSize + 1]);
			if (!messagesFile.read(tmpCharBuf.get(), tmpSize))
				return false;
			tmpCharBuf[tmpSize] = '\0';
			tmpStrM = std::string(tmpCharBuf.get());			// Читаем текст сообщения

			if (!messagesFile.read(tmpCharSize, sizeof tmpSize))
				return false;
			tmpCharBuf.reset(new char[tmpSize + 1]);
			if (!messagesFile.read(tmpCharBuf.get(), tmpSize))
				return false;
			tmpCharBuf[tmpSize] = '\0';
			tmpStrR = std::string(tmpCharBuf.get());			// Читаем логин получателя

			ChatMessage tmpMsg(tmpStrS, tmpStrM, tmpStrR);
			addMessage(tmpMsg);

		}
		messagesFile.close();
		return true;
	}
	else
		return false;
}
