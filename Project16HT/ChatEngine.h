#pragma once
#include "ChatUser.h"
#include "ChatUsersList.h"
#include "ChatMessage.h"
#include "ChatMessagesList.h"
#include "ChatStateTable.h"

//Заданная вручную таблица переходов между состояниями чата:
static const TableElement defaultTable[] =
{
    //  Текущее состояние:              Код перехода:   Новое состояние:

    // Состоялся вывод информационного сообщения
    {    statePrintHelpNotLoggedIn,     'R',            stateRegistration   }, // Регистрация нового пользователя
    {    statePrintHelpNotLoggedIn,     'I',            stateLogIn  }, // Вход в систему зарегистрированного пользователя
    {    statePrintHelpNotLoggedIn,     'E',            stateQuit   }, // Завершение программы
    {    statePrintHelpNotLoggedIn,     'H',            statePrintHelpNotLoggedIn   }, // Вывод информационного сообщения
    // Состоялась (успешно или не успешно) регистрация пользователя
    {    stateRegistration,             'R',            stateRegistration   }, // Регистрация нового пользователя
    {    stateRegistration,             'I',            stateLogIn  }, // Вход в систему зарегистрированного пользователя
    {    stateRegistration,             'E',            stateQuit   }, // Завершение программы
    {    stateRegistration,             'H',            statePrintHelpNotLoggedIn   }, // Вывод информационного сообщения
    // Состоялся (успешно или не успешно) вход в систему: состояния-заглушки для реализации ветвления в сценарии работы
    {    stateLogIn,                    '\0',           stateUserSuccessfullLogIn   }, // Пользователь вошёл
    {    stateLogIn,                    '\0',           stateUserUnsuccessfullLogIn }, // Пользователь не вошёл
    // Состоялся вывод поступивших сообщений
    {    stateReadMessages,             'M',            stateReadMessages   }, // Чтение поступивших сообщений
    {    stateReadMessages,             'W',            stateSendMessage    }, // Написание и отправка сообщения
    {    stateReadMessages,             'O',            stateLogOut }, // Выход из системы
    {    stateReadMessages,             'E',            stateQuit   }, // Завершение программы
    {    stateReadMessages,             'L',            statePrintUsersList }, // Вывод списка пользователей
    {    stateReadMessages,             'H',            statePrintHelpLoggedIn  }, // Вывод информационного сообщения
    // Состоялось написание и отправка нового сообщения
    {    stateSendMessage,              'M',            stateReadMessages   }, // Чтение поступивших сообщений
    {    stateSendMessage,              'W',            stateSendMessage    }, // Написание и отправка сообщения
    {    stateSendMessage,              'O',            stateLogOut }, // Выход из системы
    {    stateSendMessage,              'E',            stateQuit   }, // Завершение программы
    {    stateSendMessage,              'L',            statePrintUsersList }, // Вывод списка пользователей
    {    stateSendMessage,              'H',            statePrintHelpLoggedIn  }, // Вывод информационного сообщения
    // Состоялся выход пользователя из чата
    {    stateLogOut,                   'R',            stateRegistration   }, // Регистрация нового пользователя
    {    stateLogOut,                   'I',            stateLogIn  }, // Вход в систему зарегистрированного пользователя
    {    stateLogOut,                   'E',            stateQuit   }, // Завершение программы
    {    stateLogOut,                   'H',            statePrintHelpNotLoggedIn   }, // Вывод информационного сообщения
    // Состоялся успешный вход пользователя в систему
    {    stateUserSuccessfullLogIn,     'M',            stateReadMessages   }, // Чтение поступивших сообщений
    {    stateUserSuccessfullLogIn,     'W',            stateSendMessage    }, // Написание и отправка сообщения
    {    stateUserSuccessfullLogIn,     'O',            stateLogOut }, // Выход из системы
    {    stateUserSuccessfullLogIn,     'E',            stateQuit   }, // Завершение программы
    {    stateUserSuccessfullLogIn,     'L',            statePrintUsersList }, // Вывод списка пользователей
    {    stateUserSuccessfullLogIn,     'H',            statePrintHelpLoggedIn  }, // Вывод информационного сообщения
    // Состоялся неуспешный вход пользователя в систему
    {    stateUserUnsuccessfullLogIn,   'R',            stateRegistration   }, // Регистрация нового пользователя
    {    stateUserUnsuccessfullLogIn,   'I',            stateLogIn  }, // Вход в систему зарегистрированного пользователя
    {    stateUserUnsuccessfullLogIn,   'E',            stateQuit   }, // Завершение программы
    {    stateUserUnsuccessfullLogIn,   'H',            statePrintHelpNotLoggedIn   }, // Вывод информационного сообщения
    // Состоялся вывод списка пользователей
    {    statePrintUsersList,           'M',            stateReadMessages   }, // Чтение поступивших сообщений
    {    statePrintUsersList,           'W',            stateSendMessage    }, // Написание и отправка сообщения
    {    statePrintUsersList,           'O',            stateLogOut }, // Выход из системы
    {    statePrintUsersList,           'E',            stateQuit   }, // Завершение программы
    {    statePrintUsersList,           'L',            statePrintUsersList }, // Вывод списка пользователей
    {    statePrintUsersList,           'H',            statePrintHelpLoggedIn  }, // Вывод информационного сообщения
    // Состоялся вывод информационного сообщения
    {    statePrintHelpLoggedIn,        'M',            stateReadMessages   }, // Чтение поступивших сообщений
    {    statePrintHelpLoggedIn,        'W',            stateSendMessage    }, // Написание и отправка сообщения
    {    statePrintHelpLoggedIn,        'O',            stateLogOut }, // Выход из системы
    {    statePrintHelpLoggedIn,        'E',            stateQuit   }, // Завершение программы
    {    statePrintHelpLoggedIn,        'L',            statePrintUsersList }, // Вывод списка пользователей
};
static const unsigned int  TABLE_SIZE = sizeof(defaultTable) / sizeof(defaultTable[0]);


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
	void listUsers();
    void printHelpUserOnline();
};