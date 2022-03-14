#include "FineGrainedQueue.h"

Node::Node(int x)
{
    this->value = x;
    this->node_mutex = new std::mutex();
    this->next = nullptr;
}

Node::~Node()
{
    delete node_mutex;
}


FineGrainedQueue::FineGrainedQueue() : head(nullptr) 
{ 
    queue_mutex = new std::mutex();
}

FineGrainedQueue::~FineGrainedQueue()
{
    while (head != nullptr)
        pop();
    delete queue_mutex;
}

void FineGrainedQueue::push(int value)
{
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}

int FineGrainedQueue::pop()
{
    int retVal = 0;
    Node *currentNode = head, *parentNode = head;

    if (currentNode == nullptr) // TODO: Пустая очередь => должны ждать появления элемента
        return retVal;

    while (currentNode->next != nullptr)
    {
        parentNode = currentNode;
        currentNode = currentNode->next;
    }
    retVal = currentNode->value;
    if (currentNode == head) // В очереди был 1 элемент
    {
        delete head;
        head = nullptr;
    }
    else
    {
        delete currentNode;
        parentNode->next = nullptr;
    }
    return retVal;
}

int FineGrainedQueue::size() const
{
    int retVal = 0;
    Node* currentNode = head;

    if (head == nullptr)
        return retVal;
    ++retVal;
    while (currentNode->next != nullptr)
    {
        ++retVal;
        currentNode = currentNode->next;
    }
    return retVal;
}

int FineGrainedQueue::find(int value)
{
    int retVal = 0;
    Node* currentNode = head;

    if (head == nullptr)
        return -1;

    do {
        if (currentNode->value == value)
            return retVal;
        ++retVal;
        currentNode = currentNode->next;
    } while (currentNode != nullptr);

    return -1;
}

void FineGrainedQueue::insertIntoMiddle(int value, int pos)
{
    // очередь не пустая,
    // вставляется элемент в середину или конец списка, то есть вставку в начало списка не нужно рассматривать,
    // если pos больше длины списка, то нужно вставить элемент в конец списка.
    if (pos < 1)
    {
        std::cout << "[FAILED] pos = " << pos << " - не поддерживается!" << std::endl;
        return;
    }

    Node* currentNode, * parentNode, * tmpNode;
    int depth = 1; // вставка в начало списка не рассматривается
    Node* newNode = new Node(value);

    // Глобально блокируем список на время блокировки головного и первого элемента, 
    // чтобы другой поток не смог вклиниться и просунуть между ними другой элемент
    queue_mutex->lock();
    parentNode = head;
    parentNode->node_mutex->lock();
    currentNode = head->next;
    if(currentNode != nullptr)
        currentNode->node_mutex->lock();
    queue_mutex->unlock();
    
    while (currentNode != nullptr)
    {    

        if (depth == pos) // Если дошли до нужного уровня глубины
        {
            // Вставляем новый элемент, разлочиваем мьютексы и завершаем работу.
            parentNode->next = newNode; 
            parentNode->node_mutex->unlock();
            newNode->next = currentNode;
            currentNode->node_mutex->unlock();
            return;
        }

        // Если не дошли до нужного уровня глубины, идём дальше
        tmpNode = parentNode; // Для разлочивания мьютекса parentNode после того, как указатель сдвинется вперёд
        parentNode = currentNode;
        currentNode = currentNode->next;
        tmpNode->node_mutex->unlock(); // Разлочиваем мьютекс "старого" parentNode
        if (currentNode != nullptr)
            currentNode->node_mutex->lock();

        ++depth;
    }

    parentNode->next = newNode; // Дошли до конца списка, значит добавляем новый элемент в конец
    parentNode->node_mutex->unlock(); // Раз мы здесь, значит currentNode==nullptr и его мьютекс разлочивать не нужно
}

void FineGrainedQueue::remove(int value)
{
    Node* prev, * cur;
    queue_mutex->lock();

    // здесь должна быть обработка случая пустого списка

    prev = this->head;
    cur = this->head->next;

    prev->node_mutex->lock();
    // здесь должна быть обработка случая удаления первого элемента
    queue_mutex->unlock();

    if (cur) // проверили и только потом залочили
        cur->node_mutex->lock();
    while (cur)
    {
        if (cur->value == value)
        {
            prev->next = cur->next;
            prev->node_mutex->unlock();
            cur->node_mutex->unlock();
            delete cur;
            return;
        }
        Node* old_prev = prev;
        prev = cur;
        cur = cur->next;
        old_prev->node_mutex->unlock();
        if (cur) // проверили и только потом залочили
            cur->node_mutex->lock();
    }
    prev->node_mutex->unlock();
}

void FineGrainedQueue::print() const
{
    if (head == nullptr)
    {
        std::cout << "L=0: []" << std::endl;
        return;
    }
    Node* currentNode = head;
    std::cout << "L="<< size() << ":[";
    while (currentNode != nullptr)
    {
        std::cout << currentNode->value;
        if(currentNode->next != nullptr)
            std::cout << ",";
        currentNode = currentNode->next;
    }
    std::cout << "]" << std::endl;
}