# Модуль 32. TDD (Gtest, Gmock) 
# 32.6. Итоговое задание

**Задание:** создать тесты с моком объекта, имитирующим подключение к базе данных.

**Подробнее:** напишите реализацию классов (которые будут участвовать в тестах), как показано на схеме:

![](https://lms.skillfactory.ru/assets/courseware/v1/2d26d0aa699111a03f6a2315f8ded0b3/asset-v1:SkillFactory+CPLUS+2020+type@asset+block/CPLUS_32.6m_1.png)
	
Семантика (входные параметры и возвращаемые значения) методов определяются вами. Напишите тесты с использованием методологии TDD, фреймворков GTest и GMock.



## Описание

### Логика основного файла
В `main.cpp` создана локальная среда тестирования (класс `SomeTestSuite`), позволяющая упаковать мок-объект внутрь тестируемого класса.
Два тест-кейса выполняют ряд проверок для двух случаев: с пустым и не пустым именем базы данных (условно именем, так как это может быть имя файла, сетевой путь и т.п.).
Каждый кейс модержит проверки, соответствующие правильному поведению тестируемого объекта. Для демонстрации "отлова" ошибок в одином из методов тестируемого класса алгоритм "испорчен" и выдаёт неправильный результат с 50%-вероятностью.

### Описание созданных классов
1. Интерфейсный класс `DBConnectionInterface` для создания дочерних классов, обеспечивающих соединение по различным протоколам, а также для создания мок-класса.
2. Реальный класс `DBConnection` отвечает за низкоуровневое соединение с БД (работа с драйверами, сетью и т.п.). Прототип, не реализован.
3. Мок-класс `MockDBConnection` семантически совпадает с классом `DBConnection`, позволяет при взаимодействии с другими классами осуществлять обмен произвольными данными.
4. Интерфейсный класс `ClassThatUseDbInterface` для создания дочерних классов, обеспечивающих различное взаимодействие с БД, а также для создания мок-класса (за рамками данного ТЗ).
5. Реальный класс `ClassThatUseDb` отвечает за высокоуровневое взаимодействие с БД. Прототип, реализовано базовое взаимодействие с низким уровнем (`DBConnectionInterface`). Для этих методов созданы тесты, в которых реальный объект низкого уровня имитируется мок-объектом `MockDBConnection`.

### Описание выбора семантики методов
1. `DBConnectionInterface: open(), close(), execQuery()` Объект класса должен "хранить" состояние соединения с БД. Соответственно, в нём должен храниться некоторый внутренний хэндл, инициализирующийся при открытии `open` и освобождающийся при закрытии `close`. Для открытия разумно использовать, как минимум, один строковый параметр (путь к файлу/серверу/имя БД и т.п.) и возвращать информацию об успешности. При закрытии параметры обычно не требуются. Метод отправки запроса, как правило, должен содержать строковое значение запроса и возвращать строковый результат. Таким образом, имеем следующие объявления: `bool open(string), void close(), string execQuery(string)`.
2. `ClassThatUseDbInterface` Реалиозована идентичная семантика методов: `bool openConnection(string), void closeConnection(), string useConnection(string)`.