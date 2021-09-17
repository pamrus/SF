#pragma once
#include "IElectronics.h"

// =============================== //
// ==  Класс товаров с экраном  == //
// =============================== //

class WithScreen :
    virtual public IElectronics
{
public:
    WithScreen() {}; // Ввод значений с консоли
    WithScreen(string name, unsigned int price, string color, unsigned int screenSize) : IElectronics(GoodsType::WITHSCREEN, name, price, color), _screenSize(screenSize) {};
    WithScreen(unsigned int screenSize) : _screenSize(screenSize) {}; // Нужен для ромбовидного наследования
    ~WithScreen(); // Автоматически становится виртуальным, т.к. наследуется от виртуального деструктора базового класса

    unsigned int getScreenSize() const;
    virtual string ShowSpec() const override; // Вернуть значения всех свойств

protected:
    unsigned int _screenSize; // Диагональ экрана в дюймах

};

