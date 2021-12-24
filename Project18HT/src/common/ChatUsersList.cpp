#include "../../include/common/ChatUsersList.h"

ChatUsersList::ChatUsersList()
{
	loadFromBinaryFile();
}

ChatUsersList::~ChatUsersList()
{
	saveToBinaryFile();
}

bool ChatUsersList::addUser(const ChatUser& newUser)
{
	std::set<std::string>::const_iterator loginForbidden = _forbiddenLogins.find(newUser.getLogin());
	if (loginForbidden != std::end(_forbiddenLogins)) // Запрещено регистрировать нового пользователя с таким логином
		return false; // - по правилам, либо такой логин уже используется

	_usersList.insert(newUser); // Добавляем пользователя в список
	_forbiddenLogins.emplace(newUser.getLogin()); // Резервируем его логин
	return true;
}


bool ChatUsersList::login(const ChatUser& newUser) const
{
	usersListType::const_iterator userInList = findUser(newUser.getLogin());
	if (userInList == std::end(_usersList)) // Не зарегистрировано пользователя с таким логином
		return false;	
	if (userInList->compareHashes(newUser)) // Сравниваем хеши паролей при совпавшем логине
	{
		userInList->setOnline();
		return true;
	}
	return false;
}

void ChatUsersList::logout(const ChatUser& newUser) const
{
	usersListType::const_iterator userInList = findUser(newUser.getLogin());
	if (userInList != std::end(_usersList)) // Не зарегистрировано пользователя с таким логином
		userInList->setOffline();
}

/*=================================================================================
* Сохранение текущего списка пользователей в бинарный файл.
* Файл создаётся, либо перезаписывается.
* TODO: обработка исключений записи (кончилось место на диске и т.п.)
*/
bool ChatUsersList::saveToBinaryFile() const
{
	std::string fname = std::string(fileUserListPrefix) + std::string(fileUserListPostfix);
	std::ofstream usersFile(fname, std::ios::binary | std::ios::trunc);

	if (usersFile.is_open())
	{		
		std::string tmpStr;			// Буфер для получения текстовых полей структуры ChatUser
		Block tmpHash;				// Буфер для получения хеша структуры CharUser
		unsigned int tmpHashInt;	// Буфер для сохранения хеша структуры CharUser
		size_t tmpSize;				// Счётчик записываемых значений
		char* tmpCharSize = reinterpret_cast<char*>(&tmpSize);

		tmpSize = _usersList.size();
		usersFile.write(tmpCharSize, sizeof tmpSize); // Сохраняем число пользователей

		for (auto &userNum : _usersList)
		{
			tmpStr = userNum.getLogin(); // Сохраняем логин пользователя
			tmpSize = tmpStr.length();
			usersFile.write(tmpCharSize, sizeof tmpSize);
			usersFile.write(tmpStr.data(), tmpSize);

			tmpStr = userNum.getName(); // Сохраняем имя пользователя
			tmpSize = tmpStr.length();
			usersFile.write(tmpCharSize, sizeof tmpSize);
			usersFile.write(tmpStr.data(), tmpSize);

			tmpHash = userNum.getHashBinary(); // Сохраняем пароль пользователя
			for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
			{
				tmpHashInt = tmpHash[i];
				usersFile.write((char*)&tmpHashInt, sizeof tmpHashInt);
			}
			delete[] tmpHash;
		}
		usersFile.close();
		return true;
	}
	else
		return false;
}

/*=================================================================================
* Чтение списка пользователей из текстового файла.
* TODO: обработка ошибок и исключений при чтении (неверный формат и т.п.)
*/
bool ChatUsersList::loadFromBinaryFile()
{
	std::string fname = std::string(fileUserListPrefix) + std::string(fileUserListPostfix);
	std::ifstream usersFile(fname, std::ios::binary);

	if (usersFile.is_open())
	{
		std::string tmpStrL, tmpStrN;					// Буфер для сохранения текстовых полей структуры ChatUser
		size_t tmpSize;									// Счётчик считываемых значений
		char* tmpCharSize = reinterpret_cast<char*>(&tmpSize);
		std::unique_ptr<char[]> tmpCharBuf;				// Буфер для чтения текстовых величин
		unsigned int tmpHashInt = 0;					// Буфер для сохранения хеша структуры CharUser

		if ( !usersFile.read(tmpCharSize, sizeof tmpSize) )
			return false;
		size_t usersCount = tmpSize;	// Читаем число пользователей

		for (size_t userNum = 0; userNum < usersCount; ++userNum)
		{
			
			if ( !usersFile.read(tmpCharSize, sizeof tmpSize) )
				return false;
			tmpCharBuf.reset(new char[tmpSize + 1]);
			if (!usersFile.read(tmpCharBuf.get(), tmpSize))
				return false;
			tmpCharBuf[tmpSize] = '\0';
			tmpStrL = std::string(tmpCharBuf.get() );			// Читаем логин пользователя

			if (!usersFile.read(tmpCharSize, sizeof tmpSize) )
				return false;
			tmpCharBuf.reset(new char[tmpSize + 1]);
			if (!usersFile.read(tmpCharBuf.get(), tmpSize))
				return false;
			tmpCharBuf[tmpSize] = '\0';
			tmpStrN = std::string( tmpCharBuf.get() );			// Читаем имя пользователя

			Block tmpHash = new uint[SHA1HASHLENGTHUINTS];	// Новый хеш структуры CharUser
			
			for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)	// Читаем пароль пользователя
				if (!usersFile.read((char*)&tmpHashInt, sizeof(tmpHashInt)))
				{
					std::cerr << "[FAILED] to read from file: " << usersFile.rdstate() << std::endl;
					delete[] tmpHash;
					return false;
				}
				else
					tmpHash[i] = tmpHashInt;

			ChatUser tmpUser(tmpStrL, tmpStrN, tmpHash);
			addUser(tmpUser);
			_forbiddenLogins.emplace(tmpUser.getLogin()); // Резервируем логин
		}
		usersFile.close();
		return true;
	}
	else
		return false;
}

bool ChatUsersList::isUserRegistered(const std::string& login) const
{
	usersListType::const_iterator iterFound = findUser(login);
	return iterFound != std::end(_usersList);
}

std::string ChatUsersList::getNameByLogin(const std::string& login) const
{
	usersListType::const_iterator iterFound = findUser(login);
	if (iterFound != std::end(_usersList))
		return iterFound->getName();
	else
		return "";
}

size_t ChatUsersList::getNumberOfUsers() const
{
	return _usersList.size();
}

usersListType::const_iterator ChatUsersList::findUser(const std::string& login) const
{
	//ChatUser tmpUser(login); // Создаём временного пользователя для поиска по логину
	return _usersList.find(ChatUser(login));
}

ostream& operator<<(ostream& output, const ChatUsersList& userList)
{
	for (auto userNum : userList._usersList)
	{
		if(userNum.isOnline())
			output << "[" << userNum.getLogin() << "] " << userNum.getName() << " - " << "Online" << std::endl;
		else
			output << "[" << userNum.getLogin() << "] " << userNum.getName() << " - " << "Offline" << std::endl;
	}
	return output;
}
