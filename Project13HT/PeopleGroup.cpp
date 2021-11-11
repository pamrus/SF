#include "PeopleGroup.h"

// Добавление одного человека по имени без связей
void PeopleGroup::addPerson(const std::string name)
{
	addVertex(getVertexesCount(), name);
}

// Добавление людей по именам без связей
void PeopleGroup::addPeople(const std::string* names, const vertexNumberType& peopleCount)
{
	for (vertexNumberType i = 0; i < peopleCount; ++i)
	{
		addVertex(getVertexesCount(), names[i]);
	}
}

// Удаление одного человека по имени с перестройкой связей
void PeopleGroup::delPerson(const std::string& name)
{
	vertexNumberType vertexToDel = findVertexByName(name);
	Graph::delVertex(vertexToDel);
}

// Установление связи между двумя людьми, находимыми по именам
void PeopleGroup::makeFriendship(const std::string& man1, const std::string& man2)
{
	vertexNumberType vertex1 = findVertexByName(man1);
	vertexNumberType vertex2 = findVertexByName(man2);
	addEdge(vertex1, vertex2, 1);
}

// Добавление связей между одним человеком и списком
void PeopleGroup::makeFriendship(const std::string& person, const std::string* otherPeople, const vertexNumberType& friendsCount)
{
	vertexNumberType personNumber = findVertexByName(person);
	vertexNumberType newFriend = vertexWrongNumber;

	for (vertexNumberType i = 0; i < friendsCount; ++i)
	{
		newFriend = findVertexByName(otherPeople[i]);
		addEdge(personNumber, newFriend, 1);
	}
}

// Найти и вывести в консоль пары людей, колчество рукопожатий между которыми не превышает заданное число
void PeopleGroup::findCloseFriendships(const vertexNumberType& handshakesCount) const
{
	edgesLengthType** weights = findMinDistancesFloyd();

	for (vertexNumberType i = 0; i < getVertexesCount(); ++i)
	{
		for (vertexNumberType j = i + 1; j < getVertexesCount(); ++j)
			if (weights[i][j] <= handshakesCount)
				std::cout << getName(i) << " - " << getName(j) << " : " << weights[i][j] << std::endl;
	}
	std::cout << std::endl;
}



vertexNumberType PeopleGroup::getNumber(const vertexNumberType& number) const
{
	return _vertexes[number]->getNumber();
}

std::string PeopleGroup::getName(const vertexNumberType& number) const
{
	return _vertexes[number]->getName();
}

// Возвращение номера вершины во внутреннем массиве по значению поля _name вершины
vertexNumberType PeopleGroup::findVertexByName(const std::string& name) const
{
	for (vertexNumberType i = 0; i < getVertexesCount(); ++i) // Перебираем вершины
		if (name.compare(_vertexes[i]->getName()) == 0) // в поисках name
			return i;
	return vertexWrongNumber; // Если не нашли равного значения
}

// Возвращение номера вершины во внутреннем массиве по значению поля _number вершины
vertexNumberType PeopleGroup::findVertexByNumber(const vertexNumberType& number) const
{
	for (vertexNumberType i = 0; i < getVertexesCount(); ++i) // Перебираем вершины
		if (_vertexes[i]->getNumber() == number) // в поисках number
			return i;
	return vertexWrongNumber; // Если не нашли равного значения
}

// Ввод с консоли списка людей без связей
void PeopleGroup::consoleInputNames()
{
	vertexNumberType peopleCount = 0;
	std::cout << "Enter the number of people:";
	std::cin >> peopleCount;
	std::string* names = new std::string[peopleCount];

	for (vertexNumberType i = 0; i < peopleCount; ++i)
	{
		std::cout << i + 1 << ")";
		std::cin >> names[i];
	}
	addPeople(names, peopleCount);
	delete[] names;
}

// Ввод с консоли дружественных связей между сохранёнными людьми
void PeopleGroup::consoleInputFriendships()
{
	std::cout << "There are " << getVertexesCount() << " persons: ";
	std::string* friends = new std::string[getVertexesCount()];

	bool firstPerson = true;
	for (vertexNumberType i = 0; i < getVertexesCount(); ++i)
	{
		if (!firstPerson)
			std::cout << ", ";
		else
			firstPerson = !firstPerson;
		std::cout << getName(i);
	}

	vertexNumberType friendsCount = 0;
	std::cout << std::endl << "Fill friendly relations list" << std::endl;
	for (vertexNumberType i = 0; i < getVertexesCount(); ++i)
	{
		std::cout << i + 1 << "] " << getName(i) << " - Enter the number of friends:";
		std::cin >> friendsCount;
		for (vertexNumberType j = 0; j < friendsCount; ++j)
		{
			std::cout << j + 1 << ")";
			std::cin >> friends[j];
		}
		makeFriendship(getName(i), friends, friendsCount);
	}
	delete[] friends;
}
