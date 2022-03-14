#include <iostream>
#include "FineGrainedQueue.h"
#include <thread>
#include <future>

int main()
{
	setlocale(LC_ALL, "");
	srand(time(0));

	FineGrainedQueue queue;

	// Добавляем индикаторные элементы, которые не будут удаляться:
	queue.push(100);
	queue.push(200);
	queue.push(300);
	queue.push(400);
	queue.push(500);
	queue.push(600);
	
	queue.print();

	constexpr size_t threadSteps = 1000;
	
	// В первом потоке вызываем FineGrainedQueue::insertIntoMiddle, вставляющую случайные значения от 0 до 9 на случайные места:
	auto insertThreadHandle = [&]() { for (size_t i = 0; i < threadSteps; ++i) queue.insertIntoMiddle(rand() % 10, rand() % (threadSteps / 10) + 1); };
	std::packaged_task<void(void)> insertThreadTask(insertThreadHandle);
	std::future<void> insertThreadFuture = insertThreadTask.get_future();
	std::thread insertThread(std::move(insertThreadTask));
	insertThread.detach();

	// Во втором потоке вызываем FineGrainedQueue::remove, удаляющую элементы списка, равные случайным значениям от 0 до 9:
	auto removeThreadHandle = [&]() { for (size_t i = 0; i < threadSteps; ++i) queue.remove(rand() % 10); };
	std::packaged_task<void(void)> removeThreadTask(removeThreadHandle);
	std::future<void> removeThreadFuture = removeThreadTask.get_future();
	std::thread removeThread(std::move(removeThreadTask));
	removeThread.detach();

	insertThreadFuture.get();
	removeThreadFuture.get();

	// Часть добавленных элементов сохраняется:
	queue.print();

	return 0;
}