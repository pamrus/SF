#include "ChatEngine.h"

ChatEngine::ChatEngine() { }

ChatEngine::~ChatEngine() { }

void ChatEngine::mainLoop()
{
	char key;
	while (_stateMachine.getCurrentState()!= 6) // Состояние 6 = выход из чата
	{
		switch (_stateMachine.getCurrentState())
		{
		case 0:	printHelp();
			break;
		case 1: registerUser();
			break;
		case 2: logInUser();
			break;
		case 3: readMessages();
			break;
		case 4: sendMessage();
			break;
		case 5: logOutUser();
			break;
		case 7: userLoggedIn();
			break;
		case 8: wrongLogIn();
			break;
		case 9: listUsers();
			break;
		default: std::cout << "[FAILED] state not supported!" << endl;
			break;
		}
		std::cin >> key;
		std::cin.ignore(1000, '\n'); // Пропуск всех символов, кроме первого
		_stateMachine.changeState(key);
	} 
}

void ChatEngine::printHelp()
{
	std::cout << "Приветствуем! В чате зарегистрировано " << _usersList.getNumberOfUsers() << " пользователей!" << std::endl;
	std::cout << "R - Регистрация нового пользователя" << std::endl <<		// 1
		"I - Ввод данных для входа пользователя в систему" << std::endl <<	// 2
		"M - Вывод поступивших сообщений" << std::endl <<					// 3
		"L - Вывод списка зарегистрированных пользователей" << std::endl <<	// 9
		"W - Написание и отправка сообщения" << std::endl <<				// 4
		"O - Выход пользователя из чата" << std::endl <<					// 5
		"E - Завершение работы чата" << std::endl;							// 6
	std::cout << "Доступные команды: (R,I,E)." << std::endl;
}

void ChatEngine::registerUser() // R,1
{
	std::string newLogin, newName, newPassword;
	std::cout << "==Регистрация нового пользователя==" << std::endl << "Введите логин пользователя:";
	std::cin >> newLogin;
	std::cout << "Введите имя пользователя:";
	std::cin >> newName;
	std::cout << "Введите пароль пользователя:";
	std::cin >> newPassword;
	ChatUser newUser(newLogin, newName, newPassword);
	if (_usersList.addUser(newUser))
		std::cout << "Регистрация прошла успешно!" << std::endl;
	else
		std::cout << "Ошибка регистрации! Ознакомьтесь с правилами использования чата." << std::endl;
	std::cout << "Доступные команды: (R,I,E)." << std::endl;
}

void ChatEngine::logInUser() // I,2
{
	std::string newLogin, newName, newPassword;
	std::cout << "==Вход пользователя в чат==" << std::endl << "Введите логин пользователя:";
	std::cin >> newLogin;
	std::cout << "Введите пароль пользователя:";
	std::cin >> newPassword;
	ChatUser newUser(newLogin, "", newPassword);
	if (_usersList.login(newUser))
	{
		std::cout << "Вы вошли в систему!" << std::endl;
		_currentUser = newLogin;
		_stateMachine.changeStateForced(7);

		std::cout << "Доступные команды: (M,L,W,O,E)." << std::endl;
	}
	else
	{
		std::cout << "Ошибка входа! Неправильный логин или пароль." << std::endl;
		_stateMachine.changeStateForced(8);

		std::cout << "Доступные команды: (R,I,E)." << std::endl;
	}
}

void ChatEngine::readMessages() // M,3
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
		std::cout << "[" << messageSender << "]" << messageText << std::endl;
	}

	std::cout << "Доступные команды: (M,L,W,O,E)." << std::endl;
}

void ChatEngine::sendMessage() // W,4
{
	std::cout << "==Отправка сообщения==" << std::endl;
	std::string messageText = "";
	std::string messageReceiver = "";
	std::cout << "Введите логин получателя:";
	std::cin >> messageReceiver;
	std::cout << "Введите текст сообщения:";
	std::cin >> messageText;
	ChatMessage currMessage(_currentUser, messageText, messageReceiver);
	_messagesList.addMessage(currMessage);

	std::cout << "Доступные команды: (M,L,W,O,E)." << std::endl;
}

void ChatEngine::logOutUser() // O,5
{
	std::cout << "==Вы вышли из системы==" << std::endl;
	_currentUser = "";

	std::cout << "Доступные команды: (R,I,E)." << std::endl;
}

void ChatEngine::userLoggedIn() // 7
{
	std::cout << "Здравствуйте, \'" << _usersList.getNameByLogin(_currentUser) << "\'!" << std::endl;

	std::cout << "Доступные команды: (M,L,W,O,E)." << std::endl;
}

void ChatEngine::wrongLogIn() // 8
{
	//std::cout << "Доступные команды: (R,I,E)." << std::endl;
}

void ChatEngine::listUsers()
{
	std::cout << _usersList;

	std::cout << "Доступные команды: (M,L,W,O,E)." << std::endl;
}
