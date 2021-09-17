#pragma once
#include "IElectronics.h"

// ===================================== //
// ==  Класс товаров с аккумулятором  == //
// ===================================== //

class Portable : 
    virtual public IElectronics
{
public:
    Portable() {}; // Ввод значений с консоли
    Portable(string name, unsigned int price, string color, unsigned int batteryLife) : IElectronics(GoodsType::PORTABLE, name, price, color), _batteryLife(batteryLife) {};
    Portable(unsigned int batteryLife) : _batteryLife(batteryLife) {}; // Нужен для ромбовидного наследования
    ~Portable(); // Автоматически становится виртуальным, т.к. наследуется от виртуального деструктора базового класса

    unsigned int getBatteryLife() const;
    virtual string ShowSpec() const override; // Фактически не нужна, т.к. класс промежуточный, во всех дочерних - перегружается

protected:
    unsigned int _batteryLife; // ёмкость аккумулятора в мАч

};

