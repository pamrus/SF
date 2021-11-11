#include "Vertex.h"

Vertex::~Vertex()
{
}

vertexNumberType Vertex::getNumber() const
{
	return _number;
}

std::string Vertex::getName() const
{
	return _name;
}
