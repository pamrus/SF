#pragma once
#include <iostream>

using vertexNumberType = uint16_t;		// Тип для хранения количества вершин в графе и номеров вершин
constexpr vertexNumberType vertexWrongNumber = std::numeric_limits<vertexNumberType>::max();	// Неправильный номер вершины


/*==========================================================================================================================
* Класс вершины графа
* Содержит произвольные полезные данные, не участвующие непосредственно в алгоритмах поиска
*///========================================================================================================================
class Vertex
{

public:
	Vertex(const vertexNumberType& number, const std::string& name) : _number(number), _name(name) {};
	~Vertex();

	vertexNumberType getNumber() const;	
	std::string getName() const;			

private:
    vertexNumberType _number;	// Некоторое число, фактически не используется
    std::string _name;			// Имя человека, используется в классе PeopleGroup

};
