#pragma once
#include "WithScreen.h"

class TVSet final:
    public WithScreen
{
public:
    TVSet(); // ¬вод значений с консоли
    TVSet(string name, unsigned int price, string color, unsigned int screenSize) : IElectronics(GoodsType::TVSET, name, price, color), WithScreen(screenSize) {};
    ~TVSet();

protected:

};

