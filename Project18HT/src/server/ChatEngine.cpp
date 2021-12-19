#include "../../include/server/ChatEngine.h"

ChatEngine::ChatEngine() 
{ 
	for (unsigned int i = 0; i < TABLE_SIZE; ++i)
		_stateMachine.addState(defaultTable[i]);
}

ChatEngine::~ChatEngine() { }

void ChatEngine::mainLoop()
{
	char key;
	std::cout << "Приветствуем! Число зарегистрированных пользователей: " << _usersList.getNumberOfUsers() << "." << std::endl;
	while (_stateMachine.getCurrentState()!= stateQuit)
	{
		switch (_stateMachine.getCurrentState())
		{
		case statePrintHelpNotLoggedIn:	printHelp();
			break;
		case stateRegistration: registerUser();
			break;
		case stateLogIn: logInUser();
			break;
		case stateReadMessages: readMessages();
			break;
		case stateSendMessage: sendMessage();
			break;
		case stateLogOut: logOutUser();
			break;
		case stateUserSuccessfullLogIn:		// состояния-заглушки для механики таблицы переходов
			break;
		case stateUserUnsuccessfullLogIn:	// -//-
			break;
		case statePrintUsersList: listUsers();
			break;
		case statePrintHelpLoggedIn: printHelpUserOnline();
			break;
		default: std::cout << "[FAILED] state not supported!" << endl;
			break;
		}
		std::cout << "Доступные команды: (" << _stateMachine.availableKeys() << ")." << std::endl;
		std::cin >> key;
		std::cin.ignore(1000, '\n');
		_stateMachine.changeState(key);
	}
	std::cout << "Ждём Вас снова!" << std::endl;
}

void ChatEngine::printHelp() // H
{
	std::cout << "R - Регистрация нового пользователя" << std::endl <<
		"I - Ввод данных для входа пользователя в систему" << std::endl <<
		"E - Завершение работы чата" << std::endl <<
		"H - Вывод информационного сообщения" << std::endl;
}

void ChatEngine::registerUser() // R
{
	std::string newLogin, newName, newPassword;
	std::cout << "==Регистрация нового пользователя==" << std::endl << "Введите логин пользователя:";
	std::cin >> newLogin;
	std::cout << "Введите имя пользователя:";
	std::cin.clear();
	std::cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Накапливаем "все" символы до return
	std::getline(std::cin, newName);
	std::cout << "Введите пароль пользователя:";
	std::cin >> newPassword;
	ChatUser newUser(newLogin, newName, newPassword);
	if (_usersList.addUser(newUser))
		std::cout << "Регистрация прошла успешно!" << std::endl;
	else
		std::cout << "Ошибка регистрации! Ознакомьтесь с правилами использования чата." << std::endl;
}

void ChatEngine::logInUser() // I
{
	std::string newLogin, newName, newPassword;
	std::cout << "==Вход пользователя в чат==" << std::endl << "Введите логин пользователя:";
	std::cin >> newLogin;
	std::cout << "Введите пароль пользователя:";
	std::cin >> newPassword;
	ChatUser newUser(newLogin, "", newPassword);
	if (_usersList.login(newUser))
	{
		_currentUser = newLogin;
		std::cout << "Здравствуйте, \'" << _usersList.getNameByLogin(_currentUser) << "\'!" << std::endl;
		_stateMachine.changeStateForced(stateUserSuccessfullLogIn);
	}
	else
	{
		std::cout << "Ошибка входа! Неправильный логин или пароль." << std::endl;
		_stateMachine.changeStateForced(stateUserUnsuccessfullLogIn);
	}
}

void ChatEngine::readMessages() // M
{
	std::cout << "==Чтение новых сообщений==" << std::endl;
	std::string messageText = "";
	std::string messageSender = "";
	ChatMessage currMessage;
	bool msgExists = false;
	while (true)
	{
		currMessage = _messagesList.findMessage(_currentUser);
		messageText = currMessage.getMessage();
		messageSender = currMessage.getSender();
		if (messageSender == "")
		{
			if(!msgExists)
				std::cout << "У Вас нет новых сообщений." << std::endl;
			break;
		}
		if (!msgExists)
		{
			std::cout << "Сообщения для пользователя \'" << _usersList.getNameByLogin(_currentUser) << "\':" << std::endl;
			msgExists = true;
		}
		std::cout << "[" << messageSender << "]: " << messageText << std::endl;
	}
}

void ChatEngine::sendMessage() // W
{
	std::cout << "==Отправка сообщения==" << std::endl;
	std::string messageText;
	std::string messageReceiver;
	std::cout << "Введите логин получателя:";
	std::cin >> messageReceiver;
	std::cout << "Введите текст сообщения:";
	std::cin.clear();
	std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
	std::getline(std::cin, messageText);
	ChatMessage currMessage(_currentUser, messageText, messageReceiver);
	_messagesList.addMessage(currMessage);
}

void ChatEngine::logOutUser() // O
{
	ChatUser newUser(_currentUser, "", "");
	_usersList.logout(newUser);
	std::cout << "==Вы вышли из системы==" << std::endl;
	_currentUser.clear();
}

void ChatEngine::listUsers() // L
{
	std::cout << "Число зарегистрированных пользователей: " << _usersList.getNumberOfUsers() << "." << std::endl;
	std::cout << _usersList;
}

void ChatEngine::printHelpUserOnline() // H
{
	std::cout << "M - Вывод поступивших сообщений" << std::endl <<
		"L - Вывод списка зарегистрированных пользователей" << std::endl <<
		"W - Написание и отправка сообщения" << std::endl <<
		"O - Выход пользователя из чата" << std::endl <<
		"E - Завершение работы чата" << std::endl;
}
