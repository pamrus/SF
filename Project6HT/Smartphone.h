#pragma once
#include "Portable.h"
#include "WithScreen.h"

// ============================================== //
// ==            Класс смартфонов:             == //
// ==  одновременно с аккумулятором и экраном  == //
// ============================================== //

class Smartphone final :
    public Portable, public WithScreen // Перед обоими родительскими классами должен стоять вид наследования !!
{
public:
    Smartphone(); // Ввод значений с консоли
    //Smartphone(string name, unsigned int price, string color, unsigned int batteryLife, unsigned int screenSize) : Portable(name, price, color, batteryLife), WithScreen(screenSize) {};
    //Smartphone(string name, unsigned int price, string color, unsigned int batteryLife, unsigned int screenSize) : Portable(batteryLife), WithScreen(name, price, color, screenSize) {};
    // Если делать так ^ - у объекта АБК вызовется конструктор по-умолчанию, и его 3 поля не запишутся значениями!
    // - Нужно сначала вызвать именно конструктор АБК, запишутся 3 поля базового объекта, и потом уже создадутся дочерние объекты со своими полями:
    Smartphone(string name, unsigned int price, string color, unsigned int batteryLife, unsigned int screenSize) : IElectronics(GoodsType::SMARTPHONE, name, price, color), Portable(batteryLife), WithScreen(screenSize) {};
    // TODO: разобраться, как в памяти размещается эта химера! - https://chowdera.com/2021/08/20210803055556352q.html
    ~Smartphone();

    string ShowSpec() const override;

protected:

};

