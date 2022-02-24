#include "randomVector.h"



randomVector::randomVector() : _mean(0), _deviation(0), _totalSum(0.0), _additionDuration(0), _generationDuration(0) { }

randomVector::randomVector(const randomVector::iterator& start, const randomVector::iterator& stop, const randomVector& parent) : std::vector<double>(start, stop),
	_mean(parent.getMean()), _deviation(parent.getDeviation()), _totalSum(0.0)
{ 
	_generationStart = timeNow();
	_generationDuration = parent.getGenerationDuration() * (this->size() / parent.size());
}

randomVector::randomVector(size_t length, double mean, double deviation) : std::vector<double>(length), _mean(mean), _deviation(deviation), _totalSum(0.0), _additionDuration(0)
{
	std::random_device randomize; // Для инициализации вихря Мерсенна
	std::seed_seq randomSeed{ randomize(), randomize(), randomize(), randomize(), randomize(), randomize(), randomize(), randomize() };
	std::mt19937 randomIntGenerator(randomSeed);
	std::normal_distribution<double> randomFloatingGenerator(_mean, _deviation);
	_generationStart = timeNow();
	generate(begin(), end(), bind(randomFloatingGenerator, randomIntGenerator));
	_generationDuration = timeSpent(_generationStart);
}

void randomVector::operator()(const std::string& prefix)
{
	std::string msg = prefix;
	
	_additionStart = timeNow();
	_totalSum = std::accumulate(this->begin(), this->end(), 0.0);	// Непосредственно суммирование
	_additionDuration = timeSpent(_additionStart);

	msg += " Инициализация вектора длиной " + std::to_string(this->size()) + " случайными значениями: " + std::to_string(this->getGenerationDuration()) + " мс\n";
	msg += prefix + " Длительность суммирования: " + std::to_string(this->getAdditionDuration()) + " мс, значение суммы: " + std::to_string(_totalSum) + "\n";
#ifdef _DEBUG
	std::cout << msg;// << std::endl;
#endif
}

double randomVector::getMean() const
{
	return _mean;
}

double randomVector::getDeviation() const
{
	return _deviation;
}

double randomVector::getGenerationDuration() const
{
	return _generationDuration;
}

double randomVector::getAdditionDuration() const
{
	return _additionDuration;
}

double randomVector::getTotalSum() const
{
	return _totalSum;
}

// Отделяем конец вектора в новый вектор
randomVector randomVector::cutoffTail(size_t length)
{
	auto first = this->end() - length;
	auto last = this->end();
	randomVector retVal(first, last, *this);
	this->erase(first, last);
	return retVal;
}
/* Пример использования:
std::cout << "Orig: " << randomData.size() << std::endl;
auto tmp = randomData.cutoffTail(1);
std::cout << "Tail: " << tmp.size() << std::endl;
std::cout << "Remaining: " << randomData.size() << std::endl;
*/

double randomVector::at(size_t index) const
{
	return vector<double>::at(index);
}

double randomVector::operator[](size_t index) const
{
	return vector<double>::at(index);
}
