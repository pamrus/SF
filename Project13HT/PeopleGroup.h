#pragma once
#include "Graph.h"

/*==========================================================================================================================
* Класс Группа людей, дочерний от класса Graph
* Позволяет создать группу людей, задать между ними дружеские связи и проверить длину связей между ними
* В отличие от класса Graph, требуется наличие поля "std::string _name" у класса Vertex и соответствующего геттера
*///========================================================================================================================
class PeopleGroup :
    public Graph
{
public:

    PeopleGroup(const vertexNumberType& peopleCount) : Graph::Graph(peopleCount) {}

    void addPerson(const std::string name);                                     // Добавление одного человека по имени без связей
    void addPeople(const std::string* names, const vertexNumberType& peopleCount); // Добавление людей по именам без связей
    void delPerson(const std::string& name);    // Удаление одного человека по имени с перестройкой связей
    void makeFriendship(const std::string& man1, const std::string& man2);      // Установление связи между двумя людьми, находимыми по именам
    void makeFriendship(const std::string& person, const std::string* otherPeople, const vertexNumberType& friendsCount);  // Добавление связей между одним человеком и списком людей
    void findCloseFriendships(const vertexNumberType& handshakesCount) const;   // Найти и вывести в консоль пары людей, колчество рукопожатий между которыми не превышает заданное число

    vertexNumberType getNumber(const vertexNumberType& number) const;
    std::string getName(const vertexNumberType& number) const;
    vertexNumberType findVertexByName(const std::string& _name) const;          // Возвращение номера вершины во внутреннем массиве по значению поля _name вершины
    vertexNumberType findVertexByNumber(const vertexNumberType& number) const;  // Возвращение номера вершины во внутреннем массиве по значению поля _number вершины

    void consoleInputNames();       // Ввод с консоли списка людей без связей
    void consoleInputFriendships(); // Ввод с консоли дружественных связей между сохранёнными людьми

private:

};
