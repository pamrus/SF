#include <iostream>
#include "chat.h"



int main()
{
	setlocale(LC_ALL, "");
	
	Chat chat;

	std::string login1 = "User1";
	std::string password1 = "HackMe";
	
	std::string login2 = "User2";
	std::string password2 = "qwerty";

	std::cout << chat.login(login1, password1);

	chat.reg(login1, password1);
	chat.reg(login2, password2);

	std::cout << chat.login(login1, password1);
	

	std::cout << chat.login(login1, password2);

	/*
	HashTable ht;
	
	std::string login = "User";

	std::string pw = "qwerty";
	Block pwhash = sha1(pw.c_str(), pw.length());
	ht.add(login, pwhash);
	printDataHex(pwhash, SHA1HASHLENGTHUINTS);


	Block spw = ht.find(login);
	printDataHex(spw, SHA1HASHLENGTHUINTS);
	*/
	

	/*
	Chat chat;

	char* loginPtr = nullptr;
	char* pwPtr = nullptr;

	char login1[] = "user56789\0";
	char pass1[] = "1234567\0"; // f7c3bc1d808e04732adf679965ccc34ca7ae3441

	loginPtr = login1;
	pwPtr = pass1;


	std::cout << chat.login(loginPtr, pwPtr, 1);

	chat.reg(loginPtr, pwPtr, 4);

	std::cout << chat.login(loginPtr, pwPtr, 1);
	char pass2[] = "1234567\0"; // dca1f9024e545c3fb441b85cac30c7dbe1687cda
	pwPtr = pass2;
	std::cout << chat.login(loginPtr, pwPtr, 1);
	*/


	/*
	std::string pw = "qwerty";

	Block pwhash = sha1(pw.c_str(), pw.length());

	std::cout << "PW: " << pw << ", SHA1: " << *pwhash << std::endl;

	HashTable ht;

	string user = "User";

	ht.add(user, pwhash);

	Block spw = ht.find("USer");
	std::cout << "Found: " << spw << std::endl;
	*/

	return 0;
}