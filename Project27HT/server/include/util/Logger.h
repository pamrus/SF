#pragma once
#define _CRT_SECURE_NO_WARNINGS // Для std::put_time, std::localtime
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>   // localtime
#include <iomanip> // put_times
#include <mutex>
#include <string>
#include <boost/noncopyable.hpp>

/*
Класс построчного чтения/записи текстовых файлов с временными метками.
Открытие файла в режиме read+write(append) осуществляется (в закрытой функции init) при первом обращении к файлу (использован std::call_once).
Количество записанных строк запрашивается функцией getLinesCount, после её вызова количество может только увеличиться.
Чтение строки по индексу осуществляется функцией readNthLine (индексация с нуля), при её отсутствии возвращается пустая строка.
Добавление новой строки в конец файла осуществляется оператором <<.
Извлечение строк из файла не поддерживается. Все критические секции защищены внутренним мьютексом.
Закрытие файла происходит при вызове деструктора класса.
Копирование экземпляров класса запрещено путём наследования от boost::noncopyable. Для использования в нескольких потоках можно применять std::shared_ptr<Logger>.
*/
class Logger : private boost::noncopyable
{
public:
	Logger(const std::string& fName);
	~Logger();

	bool isOpen() const;
	// Функции чтения-записи в файл. При первом запуске вызывают init. При неудачном открытии возвращяют пустые результаты.
	void operator << (const std::string& str);
	size_t getLinesCount();
	std::string readNthLine(const size_t& index);

private:
	std::once_flag _initialized;	// Открытие файла при вызове первой из функций чтения/записи 
	void init();

	std::string _fName;
	std::fstream _handle;
	std::mutex _mutex;	// Защита от одновременного обращения к файлу из нескольких потоков
};

