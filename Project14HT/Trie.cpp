#include "Trie.h"

Trie::Trie()
{
	_root = new Node();
}

Trie::~Trie()
{
    delete _root;
}

// Добавление слова
void Trie::insert(const std::string& key)
{
    if (search(key)) // Не добавляем дубликаты
        return;

    Node* node = _root;
    for (unsigned char i = 0; i < key.length(); ++i) // Для каждого символа нового слова
    {
        // вычисляем индекс в алфавите через смещение относительно первой буквы
        unsigned char index = key[i] - alphabetStart;
        node = node->goDeepByIndex(index); // Проходим дальше в дереве, если символ есть и создаём новый символ, если его ещё нет.
    }
    // помечаем последний узел как лист, т.е. конец слова
    node->setEndOfWord();
}

// Итеративная функция удаления слова
void Trie::remove(const std::string& key)
{
    if (search(key) == false) // Чтобы не портить значения счётчиков, если слова в словаре нет
        return;

    // Теперь мы знаем точно, что key в дереве есть,
    // значит можно смело идти вглубь без проверок!
    Node* node = _root;
    unsigned char depth = 0;
    for (unsigned char i = 0; i < key.length(); ++i) // Для каждого символа удаляемого слова
    {
        unsigned char index = key[i] - alphabetStart;
        node = node->lookDeepByIndex(index);    // Проходим дальше в дереве
        if (!node->isVisitedOnce())             // и если данная буква содержится более чем в одном слове,
            node->decreaseVisitsCounter();      // уменьшаем счётчик слов на каждой букве
        else                                    // Если дошли до части префикса, содержащегося уже только в одном слове,
            break;                              // выходим из цикла
        ++depth;                                // сохранив глубину неуникального префикса
    }

    if (depth == key.length())                  // Если дошли до конца, значит нужно удалить короткое слово, 
    {                                           // а в дереве есть более длинные с тем же префиксом -
        node->resetEndOfWord();                 // просто сбрасываем признак
    }
    else
    {
        node = _root;                           // Иначе снова проходим до конца менее длинного слова,
        for (unsigned char i = 0; i < depth; ++i)// но останавливаемся на символ раньше
        {
            unsigned char index = key[i] - alphabetStart;
            node = node->lookDeepByIndex(index);
        }
        unsigned char index = key[depth] - alphabetStart;

        node->deleteChild(index);               // Остаток связан только с удаляемым словом, так что просто его удаляем.
    }
}

// Проверка на содержание слова
bool Trie::search(const std::string& key) const
{
    Node* node = _root;

    for (unsigned char i = 0; i < key.length(); ++i) // Для каждого символа слова
    {
        // вычисляем индекс в алфавите через смещение относительно первой буквы
        unsigned char index = key[i] - alphabetStart;
        node = node->lookDeepByIndex(index);
        // Проходим дальше в дереве, если символ есть и выходим, если его нет.
        if(node == nullptr)
            return false;
    }

    return (node != nullptr && node->isEndOfWord()); // Успех, если на длине key слово в дереве заканчивается
}

// Функция нахождения кратчайшего уникального префикса для каждого слова в дереве. 
// Ответ записывается в строку res через пробел без запятых.
std::string Trie::findMinPrefixes() const
{
    unsigned char buf[biggestWordLength];
    unsigned char ind = 0;
    std::string retVal;
    findMinPrefixes(_root, buf, ind, retVal);
    return retVal;
}

// Функция автодополнения строки по словарю
// Возвращает завершение слова, если для введённого префикса существует только один вариант окончания слова.
// Возвращает "\a", если вариантов дополнения несколько.
// Если префикс слова не найден, или оно есть, но введено полностью (дополнять нечем!) - вернётся пустая строка
std::string Trie::tryToAutoComplete(const std::string& key) const
{
    if ((key.compare("") == 0) || (this == nullptr) || (_root->isEmpty())) // Отбрасываем тривиальные варианты
        return "";

    Node* node = _root;
    std::string retVal;

    for (unsigned char i = 0; i < key.length(); ++i) // Проходим по дереву согласно переданному префиксу
    {
        unsigned char index = key[i] - alphabetStart;
        node = node->lookDeepByIndex(index);
        if (node == nullptr)
            return "";
    }

    if (node != nullptr && !(node->isEndOfWord()) && node->isVisitedOnce()) // Склеиваем остаток слова, если оно единственно
    {
        while (!node->isEmpty())
        {
            for (unsigned char i = 0; i < alphabetSize; ++i)
                if (node->childExists(i))
                {
                    retVal += (alphabetStart + i);
                    node = node->lookDeepByIndex(i);
                    break;
                }
        }
    }
    if (node != nullptr && !(node->isVisitedOnce())) // Если было несколько вариантов, сообщаем об этом управляющим символом
        return "\a";

    return retVal; 
}

// Автодополнение с выводом в консоль
bool Trie::T9(const std::string& key) const
{
    std::string autoCompleted;
    autoCompleted = tryToAutoComplete(key);

    if (autoCompleted == "")
    {
        std::cout << "T9: " << key << " - not found!" << std::endl;
        return true;
    }
    else
        if (autoCompleted == "\a")
            std::cout << "T9: " << key << " - several variants!" << std::endl;
        else
        {
            std::cout << "T9: " << key << "_" << autoCompleted << std::endl;
            return true;
        }
    return false;
}

// Вывод содержимого Trie
void Trie::print() const
{
    Node* node = _root;
    print(node, "");
    std::cout << std::endl;
}

// Построчная загрузка слов из уже отрытого файла
Trie* operator>>(std::ifstream& fileId, Trie* trie)
{
    const unsigned char wordMinLength = 3; // Добавляются только слова длиной больше 3 букв
    std::string newWord;
    bool wordIsOk;
    if (fileId.is_open())
    {
        while (std::getline(fileId, newWord)) // Цикл по строкам файла
        {
            wordIsOk = newWord.length() > wordMinLength; 
            for (unsigned char i = 0; i < newWord.length(); ++i) // Цикл по буквам строки
                if ((newWord[i] < alphabetStart) || (newWord[i] > alphabetEnd)) // Проверка слова на соответствие алфавиту
                {
                    wordIsOk = false;
                    break;
                }
            if (wordIsOk)
                trie->insert(newWord);
        }
        fileId.close();
    }
    return trie;
}

// Открытие файла по имени и построчная загрузка слов
bool Trie::loadVocabulary(const std::string& fileName)
{
    std::ifstream fileId(fileName);
    if (fileId.is_open())
    {
        fileId >> this;
        fileId.close();
        return true;
    }
    return false;
}

// Рекурсивная вспомогательная функция построчного вывода сохранённых слов
void Trie::print(Node* node, const std::string& fullWord) const
{
    if (node->isEndOfWord()) // Дошли до листа, значит слово закончилось - выводим
    {
        std::cout << fullWord << std::endl;
        if(node->isEmpty()) // Если других ветвей нет, возвращаемся
            return;
    }
    // Если не дошли до листа, ищем возможные продолжения текущего префикса
    for (unsigned char i = 0; i < alphabetSize; ++i) // Перебираем все ответвления
        if (node->childExists(i))
            print(node->lookDeepByIndex(i), fullWord + (char)(alphabetStart + i)); // Идём по ветке вглубь, добавив символ из вершины
}

// Рекурсивная вспомогательная функция нахождения кратчайшего уникального префикса для каждого слова в дереве. 
// На вход функция принимает уже построенное дерево.
void Trie::findMinPrefixes(Node* root, unsigned char buf[], unsigned char ind, std::string& res) const
{
    if (root == nullptr) 
        return;

    if (ind > biggestWordLength - 1)
        return;

    if (root->isVisitedOnce())
    {
        for (unsigned char i = 0; i < ind; ++i)
            res += buf[i];
        res += " ";
        return;
    }

    for (unsigned char i = 0; i < alphabetSize; ++i)
    {
        Node* node = root->lookDeepByIndex(i);
        if (node != nullptr)
        {
            buf[ind] = i + alphabetStart;
            findMinPrefixes(node, buf, ind + 1, res);
        }
    }
}
