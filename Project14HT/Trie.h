#pragma once
#include "Node.h"
#include <fstream>
#include <sstream>
#include <iostream>

constexpr char biggestWordLength = 35; // Максимальная длина слова для findMinPrefixes()

// ================================ //
// ==  Класс префиксного дерева  == //
// ================================ //
// Работает на произвольном непрерывном 8-битном алфавите
class Trie
{
public:
	Trie();
	~Trie();

	void insert(const std::string& key);							// Добавление слова
	void remove(const std::string& key);							// Удаление слова
	bool search(const std::string& key) const;						// Проверка на содержание слова
	std::string findMinPrefixes() const;							// Функция нахождения кратчайшего уникального префикса 
																	//		для каждого слова 

	std::string tryToAutoComplete(const std::string& key) const;	// Автодополнение строки по словарю
	bool T9(const std::string& key) const;							// Автодополнение с выводом в консоль

	void print() const;												// Вывод содержимого Trie
	friend Trie* operator>>(std::ifstream& fileId, Trie* trie);		// Построчная загрузка слов из отрытого файла
	bool loadVocabulary(const std::string& fileName);				// Открытие файла по имени и построчная загрузка слов

private:
	Node* _root;													// Указатель на корень дерева

	void print(Node* node, const std::string& prefix) const;		// Рекурсивная вспомогательная функция вывода
	void findMinPrefixes(Node* root, unsigned char buf[],			// Рекурсивная вспомогательная функция нахождения 
		unsigned char ind, std::string& res) const;					//		кратчайшего уникального префикса

};