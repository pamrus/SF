#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <cctype>

enum tableStates // Определяем перечисление состояний, в которых может находиться чат
{
    statePrintHelpNotLoggedIn,      //  Вывод информационного сообщения
    stateRegistration,              //  Регистрация нового пользователя
    stateLogIn,                     //  Ввод данных для входа пользователя в систему
    stateReadMessages,              //  Вывод поступивших сообщений
    stateSendMessage,               //  Написание и отправка сообщения
    stateLogOut,                    //  Выход пользователя из чата
    stateQuit,                      //  Завершение работы чата
    stateUserSuccessfullLogIn,      //  Пользователь успешно зашёл в чат
    stateUserUnsuccessfullLogIn,    //  Неправильный логин/пароль при входе в чат
    statePrintUsersList,            //  Вывод списка пользователей
    statePrintHelpLoggedIn          //  Вывод информационного сообщения
};

struct  TableElement // Определяем понятие перехода из одного состояния в другое по ключу
{
    tableStates  _current;
    unsigned char _key; // Код вводимого с клавиатуры символа, обеспечивающего переход из начального состояния в конечное
    tableStates  _next;
    //void (* const _functionPointer)();
};

using stateTableType = std::vector<TableElement>; // Определяем тип контейнера таблицы состояний.

/*
* Класс ChatStateTable для хранения таблицы переходов между состояниями чата
* На основе кода Thomas Matthews: https://stackoverflow.com/a/14676749
* Функционал: https://en.wikipedia.org/wiki/State-transition_table
*/
class ChatStateTable
{
public:
    ChatStateTable();
    ~ChatStateTable();

    tableStates getCurrentState() const;
    void addState(const TableElement& newState);
    void addState(const tableStates& stateFrom, const unsigned char& stateKey, const tableStates& stateTo);
    unsigned int changeState(const unsigned char& key);
    void changeStateForced(const tableStates& newState);
    std::string availableKeys() const;

private:
    stateTableType _stateTable;
    tableStates _currentState;

};
