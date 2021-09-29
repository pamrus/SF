#include <iostream>
using namespace std;

char* foo(int size)
{
    char* result = NULL;
    if (size > 0) {
        result = (char*)malloc(size * sizeof(char));
    }
    if (size > 10)
    {
        free(result);
        return NULL;
    }
    return result;
}
int main()
{
    int i;
    char* ptr;
    cin >> i;
    ptr = foo(i);
    if (i > 0 && ptr)
    {
        free(ptr);
    }
}