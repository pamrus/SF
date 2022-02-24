#include <thread>
#include <iostream>
#include "randomVector.h"
#include "util.h"

// Задаваемые константы:
constexpr size_t memoryUsage = 200; // Объём памяти под данные, МБ
constexpr double idealSum = 32768; // Число, к которому будет стремиться сумма при бесконечном числе слагаемых
constexpr double relativeError = 100; // Отношение среднеквадратического отклонения к математическому ожиданию в процентах

// Вычисляемые константы:
constexpr size_t vectorSize = memoryUsage * 1024 * 1024 / sizeof(double); // Вычисляем длину вектора
constexpr double randMean = idealSum / vectorSize; // Математическое ожидание для каждого слагаемого
constexpr double randDeviation = randMean * relativeError / 100; // Среднеквадратическое отклонение
// На интервале от нуля до удвоенного математического ожидания ->
//	при relativeError = 33.3 будут лежать 99.7% значений, а при relativeError = 100 - только 68%.

int main()
{
	setlocale(LC_ALL, "");

	std::cout << "==== Обработка в многопоточном режиме ====" << std::endl;
	unsigned int threadsCount = getKernelsCount();
	if (threadsCount == 1)
		std::cout << "Распараллеливание отсутствует." << std::endl;
	std::vector<randomVector> dataVectors(threadsCount);
	{
		std::vector<std::thread> threads;

		std::cout << "Генерация начата...";
		for (unsigned int i = 0; i < threadsCount; ++i)
			threads.push_back(std::thread([&dataVectors, threadsCount, i]() mutable { dataVectors[i] = randomVector(vectorSize / threadsCount, randMean, randDeviation); }));

		std::for_each(threads.begin(), threads.end(), [](std::thread& t) { t.join(); });
		std::cout << "выполнено!" << std::endl;
	}
	{
		std::vector<std::thread> threads;

		for (unsigned int i = 0; i < threadsCount; ++i)
			threads.push_back(std::thread(std::move(dataVectors[i]), "[" + std::to_string(i) + "]"));

		std::for_each(threads.begin(), threads.end(), [](std::thread& t) { t.join(); });
	}
	double totalSum = 0.0, totalProcessorTime = 0.0, totalProcessorAvgTime = 0.0, totalPhysTime = 0.0, totalPhysAvgTime = 0.0, tempTime = 0.0;
	for (unsigned int i = 0; i < threadsCount; ++i)
	{
		totalSum += dataVectors[i].getTotalSum();
		totalProcessorAvgTime += dataVectors[i].getAdditionDuration();
		tempTime = dataVectors[i].getGenerationDuration() + dataVectors[i].getAdditionDuration();
		totalProcessorTime += tempTime;
		totalPhysTime = std::max(totalPhysTime, tempTime);
		totalPhysAvgTime = std::max(totalPhysAvgTime, dataVectors[i].getAdditionDuration());
	}
	std::cout << "-> Суммарное процессорное время: " << totalProcessorAvgTime << " мс (подсчёт суммы) / " << totalProcessorTime << " мс (общее)" << std::endl;
	std::cout << "-> Реальное время обработки: " << totalPhysAvgTime << " мс (подсчёт суммы) / " << totalPhysTime << " мс (общее)" << std::endl;
	std::cout << "-> Вычисленная сумма: " << totalSum << ", заданное значение: " << idealSum << " (ошибка: " << abs(totalSum - idealSum) / idealSum * 100 << "%)" << std::endl;


	std::cout << "\n==== Обработка в одном потоке ====" << std::endl;
	randomVector dataVector(vectorSize, randMean, randDeviation);
	dataVector("[0]");
	tempTime = dataVector.getGenerationDuration() + dataVector.getAdditionDuration();
	std::cout << "-> Процессорное время: " << dataVector.getAdditionDuration() << " мс (подсчёт суммы) / " << tempTime << " мс (общее)" << std::endl;
	std::cout << "-> Вычисленная сумма: " << dataVector.getTotalSum() << ", заданное значение: " << idealSum << " (ошибка: " << abs(dataVector.getTotalSum() - idealSum) / idealSum * 100 << "%)" << std::endl;

	std::cout << "\n==== Увеличение производительности за счёт многопоточности ====" << std::endl;
	std::cout << "-> Ускорение подсчёта суммы: " << dataVector.getAdditionDuration() / totalPhysAvgTime << std::endl;
	std::cout << "-> Полное ускорение: " << tempTime / totalPhysTime << std::endl;

	return 0;
}