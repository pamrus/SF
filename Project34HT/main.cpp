#include <iostream>
#include <concepts>

// ============ Описание концепта ComplexConcept, объядиняющего несколько требований к свойствам класса: ============
template<typename T>
concept HasHashCalculationConvertibleToLong = requires(T v) 
{
    { v.hash() } 
    -> std::convertible_to<long>;
};

template<typename T>
concept HasToStringMethod = requires(T v)
{
    { v.toString() } 
    -> std::same_as<std::string>;
};

template<typename T>
concept ComplexConcept = HasHashCalculationConvertibleToLong<T> && HasToStringMethod<T> && !std::has_virtual_destructor<T>::value;
// ============ // ============

// Функция отображения текстового значения и хеша переменной,
// с помощью концепта 'требует' наличия у объекта класса тех свойств, которые нужны для её работы.
void printHash(ComplexConcept auto &t)
{
    std::cout << "Hash(\"" << t.toString() << "\") = " << t.hash() << std::endl;
}

// Класс, удовлетворяющий требованиям концепта ComplexConcept
class TestClassSuitable
{
public:
    TestClassSuitable(const std::string& value) : _value(value) {};
    ~TestClassSuitable() {};    // Выполняется условние на невиртуальный деструктор
    size_t hash() const { return std::hash<std::string>{}(_value); } // Выполняется условие конвертируемости в long
    std::string toString() const { return _value; } // Выполняется условие на возврат std::string
private:
    std::string _value;
};

// Класс, не удовлетворяющий требованиям концепта ComplexConcept
class TestClassUnSuitable
{
public:
    TestClassUnSuitable(const std::string& value) {};
    virtual ~TestClassUnSuitable() {};  // Виртуальный деструктор
    std::string hash() const { return std::string(); }  // Не конвертируется в long
    int toString() const { return 1; }  // Не является std::string
};

int main()
{
    setlocale(LC_ALL, "");
    TestClassSuitable correct("Test1234567890ТЕСТ");
    TestClassUnSuitable incorrect("1234");

    // Успешная компиляция:
    printHash(correct);
    // Ошибка компиляции:
    printHash(incorrect);
}