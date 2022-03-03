#pragma once
#define _CRT_SECURE_NO_WARNINGS // Для std::put_time, std::localtime
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>   // localtime
#include <iomanip> // put_times
#include <mutex>
#include <string>

class Logger
{
public:
	Logger(const std::string& fName);
	~Logger();

	bool isOpen() const;
	// Функции чтения-записи в файл. При первом запуске вызывают init. При неудачном открытии возвращяют пустые результаты.
	void operator << (const std::string& str);
	size_t getLinesCount();
	std::string readNthLine(const size_t& index); // Индексация начинается с 0

private:
	void init();	// Открытие файла

	std::string _fName;
	std::fstream _handle;
	std::mutex _mutex;	// Защита от одновременного обращения к файлу из нескольких потоков
	std::once_flag _initialized;	// Открытие файла при вызове первой из функций чтения/записи 
};

