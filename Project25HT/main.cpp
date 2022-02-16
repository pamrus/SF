#include <iostream>

#include "mySQL_API_database.h"
#include "mySQL_API_usersTable.h"
#include "mySQL_API_messagesTable.h"

constexpr const char* constDB_host = "localhost";
constexpr const char* constDB_name = "chat";
constexpr const char* constDB_user = "user";
constexpr const char* constDB_password = "password";


int main() 
{
	setlocale(LC_ALL, "");

	mySQL_API_database chatDB(constDB_host, constDB_user, constDB_password);
	chatDB.connectAnyway(constDB_name);
	mySQL_API_usersTable chatUsers(&chatDB);
	mySQL_API_messagesTable chatMessages(&chatDB);

	chatUsers.addUser("user1", "HASH1234567890", "name1", "test@test.ru");
	chatUsers.addUser("user2", "pass2", "name2", "test2@test.ru");
	chatUsers.addUser("user3", "pass3", "name3", "test3@test.ru");

	chatUsers.login("user1", "HASH1234567890");
	chatUsers.login("user2", "pass2");
	chatUsers.login("user3", "wrong");

	chatMessages.addMessage("1", "2", "1-2");
	chatMessages.addMessage("2", "3", "2-3");
	chatMessages.addMessage("3", "1", "3-1");
	chatMessages.addMessage("1", "3", "1-3");
	chatMessages.addMessage("2", "1", "2-1");
	chatMessages.addMessage("3", "2", "3-2");

	chatMessages.setRecievedMultiple(chatMessages.getUnreceivedMessagesIdsForUserId("1"));


	chatUsers.logout("user1");
	chatUsers.deleteUser("user2");



	return 0;
}