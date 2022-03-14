#pragma once
#include <iostream>
#include <mutex>

struct Node
{
public:
	Node(int x);
	~Node();
	int value;
	Node* next;
	std::mutex* node_mutex;
};

class FineGrainedQueue
{
public:
	FineGrainedQueue();
	~FineGrainedQueue();

	void push(int value);	// Thread-unsafe!
	int pop();	// Thread-unsafe!
	int size() const;	// Thread-unsafe!
	int find(int value);	// Thread-unsafe!
	void insertIntoMiddle(int value, int pos);	// Итоговое задание
	void remove(int value);	// Функция из модуля
	void print() const;	// Thread-unsafe!

private:
	Node* head;
	std::mutex* queue_mutex;
};

