#include <iostream>
#include <iomanip>
#include <vector>
#include <future>
#include "util.h"


template<typename T>
std::vector<std::vector<T>> generateRandom2dMatrix(const size_t& width, const size_t& height)
{
	constexpr T randomUpRange = 10; // Верхняя граница значений

	std::vector<std::vector<T>> retVal;
	for (size_t i = 0; i < width; ++i)
	{
		std::vector<T> row;
		for (size_t j = 0; j < height; ++j)
		{
			row.push_back(rand() % randomUpRange);
		}
		retVal.push_back(std::move(row));
	}
	
	return retVal;
}


template<typename T>
void print2dMatrix(const std::vector<std::vector<T>>& matrix)
{
	constexpr unsigned int digitsCount = 3; // Число цифр для отображения

	if (!matrix.empty())
		for (size_t i = 0; i < matrix.size(); ++i)
		{
			for (size_t j = 0; j < matrix[i].size(); ++j)
			{
				std::cout << std::setw(digitsCount) << matrix[i].at(j) << " ";
			}
			std::cout << std::endl;
		}
	else
		std::cout << "[FAILED] <print2dMatrix> : Matrix has zero size!" << std::endl;
}


// Основная функция для заполнения строки результирующей матрицы
template<typename T>
std::vector<T> calcRow(const std::vector<T>& matrix1row, const std::vector<std::vector<T>>& matrix2)
{
	std::vector<T> retVal;
	T conv = 0;
	size_t rowLength = matrix1row.size();
	for (size_t i = 0; i < matrix2[0].size(); ++i)
	{
		conv = 0;
		for (size_t j = 0; j < rowLength; ++j)
			conv += matrix1row[j] * matrix2[j][i];
		retVal.push_back(conv);
	}

	return retVal;
}


// Однопоточная функция умножения матриц
template<typename T>
std::vector<std::vector<T>> multiply2(const std::vector<std::vector<T>>& matrix1, const std::vector<std::vector<T>>& matrix2)
{
	std::vector<std::vector<T>> retVal;
	
	if(!matrix1.empty())
		if (matrix1[0].size() == matrix2.size())
			for (size_t i = 0; i < matrix1.size(); ++i)
				retVal.push_back(calcRow(matrix1[i], matrix2));
		else
			std::cout << "[FAILED] <multiply2>: Matrix A has " << matrix1[0].size() << " collumns and matrix B has " << matrix2.size() << " rows!" << std::endl;
	else
		std::cout << "[FAILED] <multiply2> : Matrix A has zero size!" << std::endl;

	return retVal;
}


// Многопоточная функция умножения матриц
std::vector<std::vector<int>> multiply2par(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2)
{
	std::vector<std::vector<int>> retVal(matrix1.size());

	if (!matrix1.empty())
		if (matrix1[0].size() == matrix2.size())
		{
			std::vector<std::future<std::vector<int>>> futures; // Для получения уведомлений о завершении функции и получения результата
			for (size_t i = 0; i < matrix1.size(); ++i) // Разбиваем первую матрицу на строки для независимой обработки в потоках
			{
				auto handle = [](size_t i, const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2)
					{ return calcRow(matrix1[i], matrix2); }; // Лямбда-функция для выполнения потоками
				std::packaged_task<std::vector<int>(size_t, const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&)> task1(handle); // Задача для получения future
				futures.push_back(std::move(task1.get_future()));
				std::thread thread(std::move(task1), i, matrix1, matrix2); // Запускаем потоки
				thread.detach();
			}
			for (size_t i = 0; i < matrix1.size(); ++i)
				retVal[i] = futures[i].get();	// Дожидаемся завершения всех потоков и сохраняем результат
		}
		else
			std::cout << "[FAILED] <multiply2>: Matrix A has " << matrix1[0].size() << " collumns and matrix B has " << matrix2.size() << " rows!" << std::endl;
	else
		std::cout << "[FAILED] <multiply2> : Matrix A has zero size!" << std::endl;

	return retVal;
}

int main()
{
	setlocale(LC_ALL, "");
	size_t N = 5000; // Размер исходных матриц: X[ЧислоЯдер * N], Y[N * 2N]
	bool generateRandomData = 1;

	unsigned int threadsCount = getKernelsCount();

	double tempTime = 0.0;
	std::chrono::high_resolution_clock::time_point startTime;

	//std::vector<std::vector<int>>	X{ {9, 3, 5}, {2, 0, 3}, {0, 1, -6} },
	//								Y{ {1, -1, -1}, {-1, 4, 7}, {8, 1, -1} }; // Константные значения для проверки правильности вычислений, переопределяются по generateRandomData
	std::vector<std::vector<int>>	X{ {1,0}, {0,1}, {1,1} },
									Y{ {1, 2, 1}, {0, 1, 2} }; // Константные значения для проверки правильности вычислений, переопределяются по generateRandomData
	std::vector<std::vector<int>> Z;

	if (generateRandomData)
	{
		size_t Xsize[]{ threadsCount, N };
		size_t Ysize[]{ N, N * 2 };
		srand(1);
		startTime = timeNow();
		X = generateRandom2dMatrix<int>(Xsize[0], Xsize[1]);
		Y = generateRandom2dMatrix<int>(Ysize[0], Ysize[1]);
		tempTime = timeSpent(startTime);
		std::cout << "Генерация заняла: " << std::setprecision(3) << tempTime / 1000 << " с" << std::endl;
	}

	std::cout << "==== Обработка в однопоточном режиме ====" << std::endl;
	startTime = timeNow();
	Z = multiply2(X, Y);
	tempTime = timeSpent(startTime);
	std::cout << "Умножение заняло: " << std::setprecision(3) << tempTime / 1000 << " с" << std::endl;
	
	if (Z[0].size() <= 20)
	{
		print2dMatrix(X);
		std::cout << "*" << std::endl;
		print2dMatrix(Y);
		std::cout << "=" << std::endl;
		print2dMatrix(Z);
	}

	std::cout << "==== Обработка в многопоточном режиме ====" << std::endl;
	startTime = timeNow();
	Z = multiply2par(X, Y);
	tempTime = timeSpent(startTime);
	std::cout << "Умножение заняло: " << std::setprecision(3) << tempTime / 1000 << " с" << std::endl;

	if (Z[0].size() <= 20)
	{
		print2dMatrix(X);
		std::cout << "*" << std::endl;
		print2dMatrix(Y);
		std::cout << "=" << std::endl;
		print2dMatrix(Z);
	}

	return 0;
}