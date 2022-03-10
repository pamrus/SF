#include "util.h"


std::chrono::high_resolution_clock::time_point timeNow()
{
	return std::chrono::high_resolution_clock::now();
}
double timeSpent(std::chrono::high_resolution_clock::time_point timeStart)
{
	std::chrono::duration<double, std::milli> duration_ms = timeNow() - timeStart;
	return duration_ms.count();
}


// Получаем число одновременно работающих потоков.
unsigned int getKernelsCount()
{
	unsigned int kernels_count = std::thread::hardware_concurrency();
	if (kernels_count < 1)
	{
		kernels_count = 1;
		std::cout << "Невозможно определить число ядер. Принимаем за 1." << std::endl;
	}
	std::cout << "Число ядер: " << kernels_count << std::endl;
	return kernels_count;
}