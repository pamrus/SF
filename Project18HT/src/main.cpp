#include <iostream>
#include "../include/server/ChatEngine.h"

int main()
{
	setlocale(LC_ALL, "");

	ChatEngine chat2021;	// Инициализация данных, загрузка списка пользователей и сообщений из файлов.
	chat2021.mainLoop();	// Основной цикл для взаимодействия с пользователями.

	return 0;				// Вызов деструкторов, сохранение данных в файлы.
}