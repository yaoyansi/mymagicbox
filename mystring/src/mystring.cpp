#include "mystring.h"

//
bool operator==(const MyString& a, const MyString& b)
{
    return a.compare(b);
}
bool operator!=(const MyString& a, const MyString& b)
{
    return !(a==b);
}
//
void swap(MyString& a, MyString& b)
{
    a.swap(b);
}
//
std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    return str.print(os);
}
