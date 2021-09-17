#include <iostream>
#include "IntegerArray.h"
using namespace std;

IntegerArray::IntegerArray(int length) : m_length{ length } // Основной конструктор
{
    if (length < 0)
        throw Bad_length(length); // <<<<<<< Исключение bad_length >>>>>>>
    if (length > 0)
        m_data = new int[length] {}; 
    // При сверхбольших размерах new тоже сгенеритует исключение "bad array new length" или "bad allocation",
    // которое корректно перехватится в main
}

IntegerArray::IntegerArray(const IntegerArray& parentIntegerArray)
{
    if (m_length < 0) // Не должно быть такого в исходном объекте, но вдруг, содержимое памяти исказилось
        throw Bad_length(m_length); // <<<<<<< Исключение bad_length >>>>>>>
    //TODO: раз пошла такая пьянка - рассмотреть случай, когда parentIntegerArray.m_length>0, а parentIntegerArray.m_data == nullptr
    m_length = parentIntegerArray.m_length;
    if (m_length > 0)
    {
        m_data = new int[m_length] {};
        for (int i{ 0 }; i < m_length; ++i)
            m_data[i] = parentIntegerArray.m_data[i];
    }
}

IntegerArray::~IntegerArray()
{
    delete[] m_data;
}

void IntegerArray::erase()
{
    delete[] m_data;
    m_data = nullptr;
    m_length = 0;
}

int& IntegerArray::operator[](int index)
{
    if (index < 0 || index >= m_length)
        throw Bad_range(index); // <<<<<<< Исключение bad_range >>>>>>>
    return m_data[index];
}

void IntegerArray::reallocate(int newLength)
{
    erase();
    if (newLength <= 0)
        return;
    m_data = new int[newLength];
    m_length = newLength;
}

void IntegerArray::resize(int newLength)
{
    if (newLength == m_length)
        return;
    if (newLength <= 0)
    {
        erase();
        return;
    }
    int* data{ new int[newLength] };
    if (m_length > 0)
    {
        int elementsToCopy{ (newLength > m_length) ? m_length : newLength };
        for (int index{ 0 }; index < elementsToCopy; ++index)
            data[index] = m_data[index];
    }
    delete[] m_data;
    m_data = data;
    m_length = newLength;
}

void IntegerArray::insertBefore(int value, int index)
{
    
    if (index < 0 || index > m_length)
        throw Bad_range(index); // <<<<<<< Исключение bad_range >>>>>>>

    int* data{ new int[m_length + 1] };

    for (int before{ 0 }; before < index; ++before)
        data[before] = m_data[before];

    data[index] = value;

    for (int after{ index }; after < m_length; ++after)
        data[after + 1] = m_data[after];

    delete[] m_data;
    m_data = data;
    ++m_length;
}

void IntegerArray::remove(int index)
{
    if (index < 0 || index >= m_length)
        throw Bad_range(index); // <<<<<<< Исключение bad_range >>>>>>>

    if (m_length == 1)
    {
        erase();
        return;
    }

    int* data{ new int[m_length - 1] };

    for (int before{ 0 }; before < index; ++before)
        data[before] = m_data[before];

    for (int after{ index + 1 }; after < m_length; ++after)
        data[after - 1] = m_data[after];

    delete[] m_data;
    m_data = data;
    --m_length;
}

int IntegerArray::findFirstElement(int value)
{
    for (int i{ 0 }; i < m_length; ++i)
    {
        if (m_data[i] == value)
        {
            return i;
        }
    }
    // Сюда можно было бы поставить выброс исключения, например тот же throw Bad_range(value);
    // Но дальше кода, который нужно аварийно исключить из выполнения, просто нет, 
    // а возврат "-1" даёт однозначное понимание, что что-то пошло не так.
    return -1; 
}

void IntegerArray::printContents()
{
    for (int i{ 0 }; i < getLength(); ++i)
        cout << m_data[i] << ' ';
    cout << '\n';
}

