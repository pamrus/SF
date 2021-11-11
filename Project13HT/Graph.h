#pragma once
#include <iostream>
#include "Vertex.h"

using edgesLengthType = uint16_t;                           // Тип для хранения расстояний между вершинами в матрице смежности
constexpr edgesLengthType pathWrongLength = std::numeric_limits<edgesLengthType>::max(); // Неправильная длина пути

constexpr float vertexesCountIncreasingMultiplier = 1.7f;   // Во сколько раз увеличиваем максимальное число вершин при исчерпании текущего хранилища

/*============================================================================================================================
* Универсальный класс ориентированного взвешенного графа на матрице смежности
* Вершины являются экземплярами класса Vertex, у которого может быть произвольное наполнение
* Никакие из полей класса Vertex не используются в методах класса Graph!
* ( = Все методы с обращением по индексу работают по номеру вершины во встроенном массиве, а не по полю _vertex класса Vertex)
*///==========================================================================================================================
class Graph
{

public:
    Graph(const vertexNumberType& maxVertexesCount);
    Graph(const Graph& other);      // Конструктор копирования
    Graph(Graph&& other) noexcept;  // Конструктор перемещения
    ~Graph();
    
    void addVertex(const vertexNumberType& vnumber, const std::string& name);                               // добавление вершины
    void addEdge(const vertexNumberType& v1, const vertexNumberType& v2, const edgesLengthType& weight);    // добавление ребра
    void delVertex(const vertexNumberType& vnumber);                                                        // удаление вершины
    void delEdges(const vertexNumberType& vnumber);                                                         // Удаление строки и столбца в матрице смежности, относящихся к вершине
    void reallocate(const vertexNumberType& maxVertexesCount);                                              // Увеличение хранилища вершин графа

    vertexNumberType getVertexesCount() const;

    edgesLengthType* findMinDistanceDecstr(const vertexNumberType& fromVert) const;   // Нахождение кратчайшего пути от одной из вершин графа до всех остальных по алгоритму Дейкстры
    edgesLengthType** findMinDistancesFloyd() const;                                  // Нахождение кратчайших путей между всеми вершинами графа по алгоритму Флойда
    
    void printDistancesFromVertex(const edgesLengthType* distances) const;              // Вывод на консоль кратчайших путей от одной из вершин графа до всех остальных
    void delete2dSquareMatrix(edgesLengthType** matrix, vertexNumberType size) const;   // Освобождение памяти из-под двумерной квадратной матрицы

private:
    bool edgeExists(const vertexNumberType& v1, const vertexNumberType& v2) const;          // Проверка на существование ребра
    bool vertexExists(const vertexNumberType& v) const;                                     // Проверка на существование вершины
    edgesLengthType** initAdjacencyMatrix(const vertexNumberType& maxVertexesCount) const;  // Создание и инициализация новой матрицы смежности
    void printAdjacencyMatrix() const;                                                      // Для отладки

    vertexNumberType _vertexesCount;        // Текущее количество вершин
    vertexNumberType _maxVertexesCount;     // Максимальное число вершин (доступно для увеличения функцией reallocate)
    edgesLengthType** _adjacencyMatrix;     // Матрица смежности

protected:
    Vertex** _vertexes;                     // Список вершин, требуется доступ в классе-потомке PeopleGroup
};
