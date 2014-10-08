#include <iostream>
#include "mystring.h"

int main()
{
    std::cout<< "hello, this is mystring plugin" << std::endl;

    MyString str0("AAA");
    std::cout << "str0="<< str0 << std::endl;

    MyString str1(NULL);
    std::cout << "str1="<< str1 << std::endl;

    //MyString str2; // compile error, cause lacking of default constructor
    //std::cout << "str2="<< str2 << std::endl;

    str0 += "BBB";
    std::cout << "str0="<< str0 << std::endl;

    MyString str3("CCC");
    str0 += str3;
    std::cout << "str0="<< str0 << std::endl;

    MyString str4("DDD");
    MyString str5("DDD");
    std::cout << "str4==str5:"<< ((str4==str5)? "true":"false") << std::endl;

    return 0;
}
