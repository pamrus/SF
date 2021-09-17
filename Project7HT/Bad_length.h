#pragma once
#include <iostream>
#include <string>
#include <exception>
using namespace std;

// ======================================================================== //
// ==  Класс исключения Bad_length - некорректно заданная длина массива  == //
// ======================================================================== //

class Bad_length :
    public length_error // По смыслу бы лучше подошёл bad_array_new_length, но ему нельзя передать значение длины
{
public:
    /* 
    Раз уж создали дочерний класс от exception,
    то протолкнём сообщение об ошибке встроенным способом по цепочке конструкторов:
    Bad_length (преобразование int в string) -> length_error(преобразование string в const char*) -> logic_error -> exception
    */
    Bad_length(const int len) : length_error( string("\n[FAILED] Wrong array length L = ") + to_string(len)) {}
    /*
    * Ну или можно так, но тогда не протолкнуть сообщение:
    virtual const char* what() const noexcept override
    {
        return "[FAILED] 201 Range check error";
    }
    */
private:
};

