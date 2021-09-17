#pragma once
#include <iostream>
#include <exception>
using namespace std;

// ============================================================= //
// ==  Класс исключения Bad_range - выход за пределы массива  == //
// ============================================================= //

class Bad_range :
    public out_of_range // Возможно, лучше бы подошли следующие классы: range_error или domain_error
{
public:
    Bad_range(const int index) : out_of_range( string("\n[FAILED] Wrong index I = " ) + to_string(index) ) {}
    /*
    virtual const char* what() const noexcept override
    {
        return "[FAILED] 201 Range check error";
    }
    */
};

