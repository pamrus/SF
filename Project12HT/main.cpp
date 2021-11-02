#include <iostream>


// Функция проверки, является ли строка inputString кратной числу K.
bool isKPeriodic(const std::string inputString, const unsigned int K)
{	
	
	if ( (K < 1)||(inputString.length() == 0) ) // Проверка на корректность входных данных, не хочется делить на 0
		return false;
	if (K > inputString.length()) // Короткая строка
		return false;
	if (inputString.length() % K != 0) // Заведомо не укладывается целое число подстрок
		return false;
	
	unsigned int repetitionCount = inputString.length() / K; // Потенциальное число повторений подстроки в строке
	std::string comparisonPattern = inputString.substr(0, K); // Выделяем из начала подстроку-шаблон для сравнения
	std::string substring; // Хранилище для остальных подстрок
	
	for (unsigned int i = 1; i < repetitionCount; i++) // Начинаем со второй подстроки
	{
		substring = inputString.substr(i * K, K); // Выделяем внутреннюю подстроку с кратным K сдвигом от начала
		if (comparisonPattern.compare(substring) != 0) // Если хотя бы одна из внутренних подстрок не совпала с начальной, значит строка уже не кратна K
			return false;
	}
	
	return true; // Если дошли до конца, значит все подстроки равны 
}





int main()
{
	std::string inputString;
	unsigned int K = 0;

	// Тестовая неинтерактивная часть проверки функции inputString

	inputString = "abcdabcdabcdabcd"; // 4 x "abcd" 
	K = 4;	// - строка кратна 4
	std::cout << "\"" << inputString << "\" is" << (isKPeriodic(inputString, K) ? " " : " not ") << K << "-periodic." << std::endl;

	inputString = "abcdabcdabcdabce"; // 3 x "abcd + "abce"
	K = 4; // - строка не кратна 4
	std::cout << "\"" << inputString << "\" is" << (isKPeriodic(inputString, K) ? " " : " not ") << K << "-periodic." << std::endl;

	inputString = "aaaaaaaaaaaaaaaa"; // 16 x "a"
	K = 1; // - любая строка из одинаковых символов кратна 1
	std::cout << "\"" << inputString << "\" is" << (isKPeriodic(inputString, K) ? " " : " not ") << K << "-periodic." << std::endl;


	// Тестовая интерактивная часть проверки функции inputString
	while (1)
	{
		std::cout << "Please enter the string (or type \"q\" symbol to exit): ";
		std::cin >> inputString;
		if (inputString.compare("q") == 0)
			return 0;
		std::cout << "Please enter the number: ";
		std::cin >> K; 
		// todo: обработка корректного ввода числа K - есть ли встроенные решения, или нужно изобретать велосипед с парсингом строки?
		std::cout << "\"" << inputString << "\" is" << (isKPeriodic(inputString, K) ? " " : " not ") << K << "-periodic." << std::endl;
	}

	return 0;
}