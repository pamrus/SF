#include "ChatUsersList.h"

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

	_table.insert(newUser); // Добавляем пользователя в список
	_forbiddenLogins.insert(newUser.getLogin()); // Резервируем его логин
	return true;
}


bool ChatUsersList::login(const ChatUser& newUser)
{
	table_type::iterator userInList = findUser(newUser.getLogin());
	if (userInList == std::end(_table)) // Не зарегистрировано пользователя с таким логином
		return false;	
	if (userInList->compareHashes(newUser)) // Сравниваем хеши паролей при совпавшем логине
	{
		//userInList->setOnline();
		return true;
	}
	return false;
}

/*=================================================================================
* Сохранение текущего списка пользователей в бинарный файл.
* Файл создаётся, либо перезаписывается.
* TODO: обработка исключений записи (кончилось место на диске и т.п.)
*/
bool ChatUsersList::saveToBinaryFile() const
{
	std::string fname = std::string(fileUserListPrefix) + std::string(fileUserListPostfix);
	std::fstream usersFile(fname, std::ios::out | std::ios::trunc);

	if (usersFile.is_open())
	{		
		std::string tmpStr;			// Буфер для получения текстовых полей структуры ChatUser
		char* tmpCharArr;			// Буфер для сохранения текстовых полей структуры ChatUser
		Block tmpHash;				// Буфер для получения хеша структуры CharUser
		unsigned int tmpHashInt;	// Буфер для сохранения хеша структуры CharUser
		unsigned int tmpSize;		// Счётчик записываемых значений

		tmpSize = _table.size();
		usersFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize)); // Сохраняем число пользователей

		for (auto userNum : _table)
		{
			tmpStr = userNum.getLogin(); // Сохраняем логин пользователя
			tmpSize = tmpStr.length();
			tmpCharArr = new char[tmpSize + 1];
			strcpy_s(tmpCharArr, tmpSize + 1, tmpStr.c_str());
			usersFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize));
			usersFile.write(reinterpret_cast<char*>(tmpCharArr), tmpSize);
			delete[] tmpCharArr;

			tmpStr = userNum.getName(); // Сохраняем имя пользователя
			tmpSize = tmpStr.length();
			tmpCharArr = new char[tmpSize + 1];
			strcpy_s(tmpCharArr, tmpSize + 1, tmpStr.c_str());
			usersFile.write(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize));
			usersFile.write(reinterpret_cast<char*>(tmpCharArr), tmpSize);
			delete[] tmpCharArr;

			tmpHash = userNum.getHashBinary(); // Сохраняем пароль пользователя
			for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)
			{
				tmpHashInt = tmpHash[i];
				usersFile.write(reinterpret_cast<char*>(&tmpHashInt), sizeof(tmpHashInt));
			}
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
	std::fstream usersFile(fname, std::ios::in);

	if (usersFile.is_open())
	{
		char* tmpCharBuf;								// Буфер для чтения текстовых полей структуры ChatUser
		std::string tmpStrL, tmpStrN;					// Буфер для сохранения текстовых полей структуры ChatUser
		Block tmpHash = new uint[SHA1HASHLENGTHUINTS];	// Новый хеш структуры CharUser
		unsigned int tmpSize;							// Счётчик считываемых значений

		if (!usersFile.read(reinterpret_cast<char*>(&tmpSize), sizeof tmpSize))
			return false;
		unsigned int usersCount = tmpSize;	// Читаем число пользователей

		for (unsigned int userNum = 0; userNum < usersCount; ++userNum)
		{
			
			if (!usersFile.read(reinterpret_cast<char*>(&tmpSize), sizeof(tmpSize)))
				return false;
			tmpCharBuf = new char[tmpSize + 1];
			if (!usersFile.read(tmpCharBuf, tmpSize))
					return false;
			tmpCharBuf[tmpSize] = 0;
			tmpStrL = tmpCharBuf;			// Читаем логин пользователя
			delete[] tmpCharBuf;

			if (!usersFile.read(reinterpret_cast<char*>(&tmpSize), sizeof tmpSize))
				return false;
			tmpCharBuf = new char[tmpSize + 1];
			if (!usersFile.read(tmpCharBuf, tmpSize))
				return false;
			tmpCharBuf[tmpSize] = 0;
			tmpStrN = tmpCharBuf;			// Читаем имя пользователя
			delete[] tmpCharBuf;

			for (unsigned int i = 0; i < SHA1HASHLENGTHUINTS; ++i)	// Читаем пароль пользователя
				if (!usersFile.read(reinterpret_cast<char*>(&tmpHash[i]), sizeof tmpSize))
					return false;

			ChatUser tmpUser(tmpStrL, tmpStrN, tmpHash);
			addUser(tmpUser);
		}
		usersFile.close();
		return true;
	}
	else
		return false;
}

bool ChatUsersList::isUserRegistered(std::string login)
{
	table_type::const_iterator iterFound = findUser(login);
	return iterFound != std::end(_table);
}

table_type::iterator ChatUsersList::findUser(const std::string& login)
{
	ChatUser tmpUser(login); // Создаём временного пользователя для поиска по логину
	return _table.find(tmpUser);
}
