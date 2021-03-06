# Модуль 34. Новшества C++ 17. Всё, что нужно знать про C++20
# 34.7. Итоговое задание

**Ваша задача** - потренироваться в написании концептов.

Напишите концепт под названием `ComplexConcept<T>`, который будет накладывать следующие ограничения на тип:
1. Наличие у типа T метода hash(), который возвращает тип, конвертируемый в long;
2. Наличие у типа T метода toString(), который возвращает std::string;
3. Отсутствие у типа T виртуального деструктора.


## Описание

Концепт `ComplexConcept` реализован, как объединение трех условий (т.к. условия по-отдельности могут пригодиться для другой задачи).
Написана функция `printHash`, принимающая аргумент типа, удовлетворяющего условиям концепта.
Созданы классы `TestClassSuitable` и `TestClassUnSuitable`: соответствующий и не соответствующий концепту.
В функции `main` экземпляры классов передаются в функцию `printHash` для демонстрации правильности кода.


Решение: Александр Павлинов 2022.04.14
