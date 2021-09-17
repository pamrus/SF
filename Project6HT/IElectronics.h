#pragma once
#include <iostream>
#include <string>
using namespace std;

enum GoodsType // Тип инициализируется в конструкторе базового класса
{
	DEFAULT = 0,	// getType + , constr x
	PORTABLE,		// getType + , constr x
	WITHSCREEN,		// getType + , constr x
	SMARTPHONE,		// getType + , constr +
	PLAYER,			// getType + , constr +
	TVSET			// getType + , constr +	
};

// ====================================== //
// ==  Базовый класс для всех товаров  == //
// ====================================== //

class IElectronics // -АБК, но не совсем интерфейс, так как содержит члены (они будут у всех потомков и без них не обойтись)
{
public:
	IElectronics() = default;  // Пустой конструктор по-умолчанию
	IElectronics(GoodsType goodsType, string name, unsigned int price, string color) : _goodsType(goodsType), _name(name), _price(price), _color(color) {}; // Основной конструктор для всех потомков
	virtual ~IElectronics() = 0; // Чистый виртуальный деструктор для удаления по всей цепочке наследования и превращения IElectronics в АБК

	virtual string getType() const; // GoodsType -> string
	virtual string getName() const;
	virtual unsigned int getPrice() const;
	virtual string getColor() const;
	virtual string ShowSpec() const; // Вернуть значения всех свойств - полиморфная, 
	//делать её чистой виртуальной нельзя, так как её использует оператор <<, а его разумнее определить один раз в АБК:
	friend ostream& operator<<(ostream& output, const IElectronics* iElectronics); // Потоковый вывод 
	//						- одинаковый для всех потомков (использует полиморфную функцию ShowSpec)
	virtual void changePrice(unsigned int newPrice); // Поменять цену на константное значение
	virtual bool makeDiscount(unsigned int discount); // Прозвести уменьшение цены на размер скидки в процентах

protected:
	string _name; // "Марка_модель"
	unsigned int _price;
	string _color;
	GoodsType _goodsType;
};

