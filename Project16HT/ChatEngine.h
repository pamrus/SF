#pragma once
#include "ChatUser.h"
#include "ChatUsersList.h"
#include "ChatMessage.h"
#include "ChatMessagesList.h"
#include "ChatStateTable.h"

/*
* Класс ChatEngine обеспечивает интерфейс чата для одного пользователя в одно время.
*
*/
class ChatEngine
{
public:
	ChatEngine();
	~ChatEngine();

	void mainLoop();					// Основной цикл чата, функционирующий через машину состояний

private:
	ChatMessagesList _messagesList;		// Список отправленных, но ещё не принятых сообщений
	ChatUsersList _usersList;			// Список зарегистрированных пользователей
	std::string _currentUser;			// Логин активного в настоящий момент пользователя
	ChatStateTable _stateMachine;		// Машина состояний чата

	// Набор функций, обеспечивающих интерфейс пользователя для каждого из состояний
	void printHelp();
	void registerUser();
	void logInUser();
	void readMessages();
	void sendMessage();
	void logOutUser();
	void userLoggedIn();
	void wrongLogIn();
	void listUsers();

};
//TODO: перевести номера состояний на constexpr-псевдонимы?