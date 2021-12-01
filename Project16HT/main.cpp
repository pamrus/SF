#include <iostream>
#include "ChatUsersList.h"
#include "ChatUser.h"

int main()
{
	setlocale(LC_ALL, "");

	ChatUsersList list;

	ChatUser user1("pam", "Pavlinov", "234");
	list.addUser(user1);
	user1 = ChatUser("pup", "Pupkin", "345");
	list.addUser(user1);


	std::cout << list.isUserRegistered("vano");

	user1 = ChatUser("sid", "Sidorov", "456");
	list.addUser(user1);

	std::cout << list.isUserRegistered("sid");

	std::cout << list.isUserRegistered("pam");
	
	user1 = ChatUser("pam", "Pavlinov", "234");
	std::cout << list.login(user1);


	return 0;
}