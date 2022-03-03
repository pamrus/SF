#include "../../include/util/Logger.h"


Logger::Logger(const std::string& fName) : _fName(fName) { }

Logger::~Logger()
{
	if (_handle.is_open())
	{
		_handle.close();
#ifdef _DEBUG
		std::cout << "[  OK  ] Logging stopped." << std::endl;
#endif
	}
}

bool Logger::isOpen() const
{
	return _handle.is_open();
}

void Logger::operator << (const std::string& str)
{
	std::call_once(_initialized, &Logger::init, this);
	if (!_handle.is_open())
		return;

	_mutex.lock();
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	_handle << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "]" << str << std::endl;
	_mutex.unlock();
}

size_t Logger::getLinesCount()
{
	std::call_once(_initialized, &Logger::init, this);
	if (!_handle.is_open())
		return 0;

	std::string tmpStr;
	size_t retVal = 0;
	_mutex.lock();
	_handle.seekg(0);
	while (1)
	{
		_handle >> tmpStr; // Читаем до первого пробела
		if (_handle.eof() || _handle.bad())
			break;
		else
		{
			_handle.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропускаем остаток до конца строки
			++retVal;
		}
	}
	_handle.clear();
	_handle.seekg(0);
	_mutex.unlock();
	return retVal;
}

std::string Logger::readNthLine(const size_t& index)
{
	std::call_once(_initialized, &Logger::init, this);
	if (!_handle.is_open())
		return std::string();

	std::string tmpStr;
	size_t retVal = 0;
	_mutex.lock();
	_handle.seekg(0);
	if (index > 0)
	{
		for (size_t i = 0; i <= index - 1; ++i)
		{
			_handle >> tmpStr;
			if (_handle.eof() || _handle.bad())
			{
				_handle.clear();
				_handle.seekg(0);
				_mutex.unlock();
				return std::string();
			}
			else
			{
				_handle.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				++retVal;
			}
		}
	}
	std::getline(_handle, tmpStr);

	_handle.clear();
	_handle.seekg(0);
	_mutex.unlock();
	return tmpStr;
}

void Logger::init()
{
	_handle.open(_fName, std::ios_base::in | std::ios_base::out | std::ios_base::app);
#ifdef _DEBUG
	if (_handle.is_open())
		std::cout << "[  OK  ] Logging started..." << std::endl;
	else
		std::cout << "[FAILED] Unable to open file '" << _fName << "'" << std::endl;
#endif
}

