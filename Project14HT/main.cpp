#include <iostream>
#include <string>
#include "Trie.h"

/*==========================================================================================================================
* Тестовая интерактивная программа проверки поставленной задачи.
* "ЗАДАЧА: реализовать программу, выполняющую автодопление введенного слова по изначально заданному словарю."
* На приведённом словаре приложение использует ~150МБ ОЗУ,
*	загрузка и очистка без профилирования занимают ~5с на "медленном" ПК
*///========================================================================================================================


int main()
{
	setlocale(LC_ALL, "");

	Trie* trie = new Trie();							// Создание объекта класса Префиксное дерево
	
	std::string vocabularyFileName = "english-words.95";// Имя файла словаря (взят из http://wordlist.aspell.net/)
	// ~ 2.5 МБ, ~220 000 слов (читаются только слова длиной более трёх букв и входящие в алфавит)

	trie->loadVocabulary(vocabularyFileName);			// Загружаем все слова длиной >= 4 букв
	//std::ifstream fileId(vocabularyFileName);
	//fileId >> trie;									//	или так

	//Дополнительный функционал:
	/*
	std::cout << trie->search("errorful") << std::endl;	// Проверка на наличие слова в дереве
	trie->remove("errorful");							// Удаление слова из дерева
	std::cout << trie->search("errorful") << std::endl;	// "0", если слово удалилось
	*/
	//trie->print();										// Вывод полного списка сохранённых слов
	//std::cout << trie->findMinPrefixes();				// Вывод кратчайших уникальных префиксов для каждого слова


	std::cout << std::endl << "Autocompletion: please, input prefix or \'exit\' to stop" << std::endl;
	std::string prefix;
	do
	{
		std::cin >> prefix;
		trie->T9(prefix);								// T9(string) выводит результат на экран
														// Для получения данных можно использовать string Trie::tryToAutoComplete(string)
	} 
	while (prefix.compare("exit") != 0);
	
	delete trie;

	return 0;
}