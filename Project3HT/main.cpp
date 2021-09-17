#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "");

    const char alphabetLength = 'z' - 'a' + 1;
    int stringLength = 0;
    char operation = 0;
    int decipherFlag = 0;
    int x = 0;
    char *arr;
    
    while (1)
    {

        cout << "Введите код операции (c-шифрование, d-дешифровка, другой символ-выход): ";
        cin >> operation;
        if ((operation != 'c') && (operation != 'd'))
            break;
        if (operation == 'd')
            decipherFlag = 1;

        cout << "Введите число символов в строке: ";
        cin >> stringLength;
        cout << "Последовательно введите " << stringLength << " символов строки: " << endl;
        arr = new char[stringLength + 1];
        for (int i = 0; i < stringLength; i++)
        {
            cin >> arr[i];
        }
        arr[stringLength] = '\0';

        cout << "Введите сдвиг шифра: ";
        cin >> x;
        x = x % alphabetLength;
        if (decipherFlag)
            x = -x;

        for (int i = 0; i < stringLength; i++)
        {
            if ((arr[i] >= 'a') && (arr[i] <= 'z'))
            {
                arr[i] = 'a' + (arr[i] - 'a' + x + (1 - (arr[i] - 'a' + x) / alphabetLength) * alphabetLength) % alphabetLength;
            }
            if ((arr[i] >= 'A') && (arr[i] <= 'Z'))
            {
                arr[i] = 'A' + (arr[i] - 'A' + x + (1 - (arr[i] - 'A' + x) / alphabetLength) * alphabetLength) % alphabetLength;
            }
        }
        cout << arr << endl;
        delete[] arr;
    }
    return 0;
}