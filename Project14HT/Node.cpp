#include "Node.h"

Node::Node() : _isEndOfWord(false), _visitedTimes(0)
{
    _children = new Node * [alphabetSize];
    for (unsigned char i = 0; i < alphabetSize; ++i)
        _children[i] = nullptr;
}

// Рекурсивно освобождаем память, занятую всеми потомками и удаляем сам узел
Node::~Node()
{
    for (unsigned char i = 0; i < alphabetSize; ++i)
        if (_children[i] != nullptr)
            delete _children[i];
    delete[] _children;
}

// Добавление потомка
void Node::addChild(const unsigned char& index)
{
    _children[index] = new Node();
}

// Удаление потомка и очистка указателя на него
void Node::deleteChild(const unsigned char& index)
{
    Node* node = lookDeepByIndex(index);
    if(node != nullptr)
        delete node; // Деструктор потянет за собой деструкторы всех его потомков
    _children[index] = nullptr;
}

// Возвращает false, если данный узел имеет хотя бы одного потомка
bool Node::isEmpty() const
{
    for (unsigned char i = 0; i < alphabetSize; ++i)
        if ((this->_children[i]) != nullptr)
            return false;
    return true;
}

// Проверка на существования потомка по коду
bool Node::childExists(const unsigned char& index) const
{
    return (_children[index] != nullptr);
}

// Проход на один узел вглубь по индексу с увеличением счётчика.
// Если детей с префиксом нет, создаем новый узел.
Node* Node::goDeepByIndex(const unsigned char& index)
{
    Node* node = this;

    // Если детей с таким префиксом нет, то создаем новый узел
    if (!node->childExists(index))
        node->addChild(index);

    node = node->_children[index];
    node->increaseVisitsCounter(); // Увеличиваем число слов, содержащих данный префикс

    return node;
}

// Просмотр на один узел вглубь по индексу без увеличения счётчика.
Node* Node::lookDeepByIndex(const unsigned char& index)
{
    if ((this == nullptr) || (this->_children == nullptr)) // TODO: возможно, излишние проверки
        return nullptr;
    return _children[index];
}

void Node::setEndOfWord()
{
    _isEndOfWord = true;
}

void Node::resetEndOfWord()
{
    _isEndOfWord = false;
}

bool Node::isEndOfWord() const
{
    return _isEndOfWord;
}

void Node::increaseVisitsCounter()
{
    ++_visitedTimes;
}

void Node::decreaseVisitsCounter()
{
    if(_visitedTimes > 0)
        --_visitedTimes;
}

bool Node::isVisitedOnce() const
{
    return (_visitedTimes == 1);
}
