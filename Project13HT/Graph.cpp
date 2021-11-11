#include "Graph.h"

Graph::Graph(const vertexNumberType& maxVertexesCount) : _maxVertexesCount(maxVertexesCount)
{
    _vertexesCount = 0;
    _vertexes = new Vertex * [_maxVertexesCount];
    _adjacencyMatrix = initAdjacencyMatrix(_maxVertexesCount);
}

// Конструктор копирования
Graph::Graph(const Graph& other)
{
    _maxVertexesCount = other._maxVertexesCount;
    _vertexes = new Vertex * [_maxVertexesCount];
    _vertexesCount = other._vertexesCount;
    _adjacencyMatrix = initAdjacencyMatrix(_maxVertexesCount);
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
    {
        _vertexes[i] = other._vertexes[i];
        for (vertexNumberType j = 0; j < _maxVertexesCount; ++j)
            _adjacencyMatrix[i][j] = other._adjacencyMatrix[i][j];
    }
}

// Конструктор перемещения, noexcept - для оптимизации при использовании стандартных контейнеров
Graph::Graph(Graph&& other) noexcept
{
    _maxVertexesCount = other._maxVertexesCount;
    other._maxVertexesCount = 0;
    _vertexesCount = other._vertexesCount;
    other._vertexesCount = 0;
    _vertexes = other._vertexes;
    other._vertexes = nullptr;
    _adjacencyMatrix = other._adjacencyMatrix;
    other._adjacencyMatrix = nullptr;
}

Graph::~Graph()
{
    delete2dSquareMatrix(_adjacencyMatrix, _vertexesCount);
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        delete _vertexes[i];
    delete[] _vertexes;
}

// добавление вершины
void Graph::addVertex(const vertexNumberType& vnumber, const std::string& name)
{
    if (_vertexesCount >= _maxVertexesCount) // Нужно увеличить объём хранилища в ~vertexesCountIncreasingMultiplier раз
        reallocate((unsigned int)((float)_maxVertexesCount * vertexesCountIncreasingMultiplier));

    Vertex* vertex = new Vertex(vnumber, name);
    _vertexes[_vertexesCount] = vertex;
    _adjacencyMatrix[_vertexesCount][_vertexesCount] = 0; // Связываем вершину с самой собой
    ++_vertexesCount;
}

// добавление ребра
void Graph::addEdge(const vertexNumberType& v1, const vertexNumberType& v2, const edgesLengthType& weight)
{
    _adjacencyMatrix[v1][v2] = weight;
    _adjacencyMatrix[v2][v1] = weight;
}

// удаление вершины по индексу в массиве, а не по _vertexes[i]->getNumber()!
void Graph::delVertex(const vertexNumberType& vnumber)
{
    if (vnumber >= _vertexesCount) // В графе нет такого элемента
        return;

    delEdges(vnumber);

    Vertex* vertexToDel = _vertexes[vnumber];

    for (vertexNumberType i = vnumber; i < _vertexesCount - 1; ++i)
    {
        _vertexes[i] = _vertexes[i + 1];
    }
    delete vertexToDel;
    --_vertexesCount;
}

// Удаление строки и столбца в матрице смежности, относящихся к вершине
void Graph::delEdges(const vertexNumberType& vnumber)
{
    //printAdjacencyMatrix();
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        for (vertexNumberType j = vnumber; j < _vertexesCount - 1; ++j)
            _adjacencyMatrix[i][j] = _adjacencyMatrix[i][j + 1]; // Сдвигаем кусок ниже vnumber на один шаг вверх
    // Теперь последняя строка содержит старый мусор
    //printAdjacencyMatrix();

    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        _adjacencyMatrix[i][_vertexesCount - 1] = pathWrongLength; // Делаем последнюю строку "не ведущей никуда"
    // Теперь матрица становится прямоугольной(_vertexesCount х _vertexesCount - 1)
    //printAdjacencyMatrix();

    for (vertexNumberType i = vnumber; i < _vertexesCount - 1; ++i)
        for (vertexNumberType j = 0; j < _vertexesCount - 1; ++j)
            _adjacencyMatrix[i][j] = _adjacencyMatrix[i + 1][j]; // Сдвигаем кусок справа vnumber на один шаг влево
    // Теперь последний столбец содержит старый мусор
    //printAdjacencyMatrix();

    for (vertexNumberType i = 0; i < _vertexesCount - 1; ++i)
        _adjacencyMatrix[_vertexesCount - 1][i] = pathWrongLength; // Делаем последний столбец "не ведущим никуда"
    
    // -> Из матрицы смежности извлечены столбец и строка, она готова к добавлению новых вершин
    //printAdjacencyMatrix();
}

// Увеличение хранилища вершин графа
void Graph::reallocate(const vertexNumberType& maxVertexesCount)
{
    if (maxVertexesCount < _maxVertexesCount)
        return; // Уменьшать не будем никогда
    _maxVertexesCount = maxVertexesCount;
    if (_maxVertexesCount < 2)
        _maxVertexesCount = 2; // Иначе так и останется <1

    Vertex** newVertexes = new Vertex * [_maxVertexesCount];
    edgesLengthType** newAdjacencyMatrix = initAdjacencyMatrix(_maxVertexesCount); // Создаём пустые массивы

    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
    {
        newVertexes[i] = _vertexes[i];
        for (vertexNumberType j = 0; j < _vertexesCount; ++j)
            newAdjacencyMatrix[i][j] = _adjacencyMatrix[i][j]; // Копируем старые значения
    }

    delete2dSquareMatrix(_adjacencyMatrix, _vertexesCount);
    delete[] _vertexes;

    _vertexes = newVertexes; // Заменяем старые массивы новыми
    _adjacencyMatrix = newAdjacencyMatrix;
}

vertexNumberType Graph::getVertexesCount() const
{
    return _vertexesCount;
}

// Нахождение кратчайшего пути от одной из вершин графа до всех остальных по алгоритму Дейкстры
edgesLengthType* Graph::findMinDistanceDecstr(const vertexNumberType& fromVert) const
{
    edgesLengthType* distances = new edgesLengthType[_vertexesCount];  // массив меток
    bool* passed = new bool[_vertexesCount]; // признак, что вершина окрашена
    
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
    {
        distances[i] = pathWrongLength;// инициализация меток
        passed[i] = false; // все неокрашены изначально
    }

    distances[fromVert] = 0;

    vertexNumberType currentVertexNum = fromVert; // принимаем стартовую вершину за текущую
    edgesLengthType min_dist = 0;

    while (min_dist != pathWrongLength)
    {
        passed[currentVertexNum] = true; // окрашиваем текущую

        for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        {
            if (edgeExists(currentVertexNum, i))
            {
                // для смежных ребер пересчитываем метки
                edgesLengthType newPath = distances[currentVertexNum] + _adjacencyMatrix[currentVertexNum][i];
                distances[i] = (newPath < distances[i]) ? newPath : distances[i];
            }
        }

        min_dist = pathWrongLength;
        for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        {
            // выбираем новую текущую вершину
            if (vertexExists(i) && !passed[i] && distances[i] < min_dist) // выбор новой вершины
            {
                min_dist = distances[i];
                currentVertexNum = i;
            }
        }
    }
    delete[] passed;

    return distances;
}

// Нахождение кратчайших путей между всеми вершинами графа по алгоритму Флойда
edgesLengthType** Graph::findMinDistancesFloyd() const
{
    edgesLengthType** weights = new edgesLengthType * [_vertexesCount]; // матрица путей
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
    {
        weights[i] = new edgesLengthType[_vertexesCount];
        for (vertexNumberType j = 0; j < _vertexesCount; ++j) // инициализация матрицы
            weights[i][j] = _adjacencyMatrix[i][j];
    }

    for (vertexNumberType k = 0; k < _vertexesCount; ++k) // итерации по k
    {
        for (vertexNumberType i = 0; i < _vertexesCount; ++i)
        {
            if (i == k)
                continue;
            for (vertexNumberType j = 0; j < _vertexesCount; ++j)
            {
                //if (j == k)
                //    continue;  // Сомнительная оптимизация - две инструкции на каждой итерации * N^3 избавляют от ~8 инструкций * N^2
                edgesLengthType newPath = pathWrongLength;
                if ((weights[i][k] != pathWrongLength) && (weights[k][j] != pathWrongLength)) // Если пусть из i в j через k существует
                {
                    newPath = weights[i][k] + weights[k][j]; // Считаем суммарный путь через вершину kfd
                    weights[i][j] = (newPath < weights[i][j]) ? newPath : weights[i][j]; // Обновляем, если новый путь короче
                }
            }
        }
    }

    return weights;
}

// Вывод на консоль кратчайших путей от одной из вершин графа до всех остальных
void Graph::printDistancesFromVertex(const edgesLengthType* distances) const
{
    for (vertexNumberType i = 0; i < _vertexesCount; ++i) // вывод результата
    {
        std::cout << "V" << i << "(" << _vertexes[i]->getName() << ")" << ": " << distances[i] << ", ";
    }
    std::cout << std::endl;
}

// Освобождение памяти из-под двумерной квадратной матрицы
void Graph::delete2dSquareMatrix(edgesLengthType** matrix, vertexNumberType size) const
{
    for (vertexNumberType i = 0; i < size; ++i)
        if(matrix[i] != nullptr)
            delete[] matrix[i];
    
    if(matrix != nullptr)
        delete[] matrix;
}

// проверка существования ребра
bool Graph::edgeExists(const vertexNumberType& v1, const vertexNumberType& v2) const
{
    return _adjacencyMatrix[v1][v2] != pathWrongLength;
}

// проверка существования вершины
bool Graph::vertexExists(const vertexNumberType& v) const
{
    if ((v >= 0) && (v < _vertexesCount))
        return true;
    else
        return false;
}

// Создание и инициализация новой матрицы смежности
edgesLengthType** Graph::initAdjacencyMatrix(const vertexNumberType& maxVertexesCount) const
{
    edgesLengthType** newAdjacencyMatrix = new edgesLengthType * [maxVertexesCount];
    for (vertexNumberType i = 0; i < maxVertexesCount; ++i)
    {
        newAdjacencyMatrix[i] = new edgesLengthType[maxVertexesCount];
        for (vertexNumberType j = 0; j < maxVertexesCount; ++j)
            newAdjacencyMatrix[i][j] = pathWrongLength; // Никаких вершин нет, ничто ни с чем не связано
    }
    return newAdjacencyMatrix;
}

// Для отладки
void Graph::printAdjacencyMatrix() const
{
    for (vertexNumberType i = 0; i < _vertexesCount; ++i)
    {
        for (vertexNumberType j = 0; j < _vertexesCount; ++j)
            if (_adjacencyMatrix[i][j] == vertexWrongNumber)
                std::cout << "X" << " ";
            else
                std::cout << _adjacencyMatrix[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
