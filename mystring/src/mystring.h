#include <cstring>
#include <cstddef>
#include <iostream>

class MyString
{
public:
    //--------------- constructor and destructor ----------------
    //explicit MyString() // Avoid gratuitous default constructors (Item4 in <<More Effective C++>>)

    explicit MyString(const char* str)
    {
        if(str == NULL)
        {
            m_str = new char[1];
            *m_str = '\0';
        }else{
            m_str = new char[strlen(str) + 1];
            memcpy(m_str, str, strlen(str)+1);
        }
    }
    //
    MyString(const MyString& o)
    {
        m_str = new char[strlen(o.m_str) + 1];
        memcpy(m_str, o.m_str, strlen(o.m_str) + 1);
    }
    //
    ~MyString()// Delcare ~MyString as virtual if it is going to be inherited.
    {
        delete[] m_str;
        m_str = NULL;
    }

    //-------------------- operator -----------------------
    MyString& operator=(const MyString& o)
    {
        MyString tmp(o);
        swap(tmp);
        return *this;
    }

    MyString& operator+=(const char* o)
    {
        if(strlen(o) == 0)
            return *this;

        char* tmp = new char[strlen(m_str) + strlen(o) + 1];
        memcpy(tmp,               m_str,   strlen(m_str));
        memcpy(tmp+strlen(m_str), o,       strlen(o) + 1);// copy '\0'

        std::swap(m_str, tmp);

        delete[] tmp;

        return *this;
    }

    MyString& operator+=(const MyString& o)
    {
        return operator+=(o.m_str);
    }

    //------------------ log -------------------
    std::ostream& print(std::ostream &os) const
    {
        return os<< "Debug: " << m_str;
    }

    // utility
    void swap(MyString& o)
    {
        std::swap(m_str, o.m_str);
    }

    bool compare(const MyString& o) const
    {
        return (strcmp(m_str, o.m_str) == 0);
    }
protected:
    char* m_str;
};
//-------------------------------------------------------
// The following operators should be declared as global functions:
// ==, !=, >, <, >=, <=, <<,
// (I take shared_ptr as a reference in <<Draft Technical Report on C++ Library Extensions>>, P19)
bool operator==(const MyString& a, const MyString& b);
bool operator!=(const MyString& a, const MyString& b);
// MyString I/O
std::ostream& operator<<(std::ostream& os, const MyString& str);

// Declare a global swap function for MyString.
// MyString specialized algorithms
// (I take shared_ptr as a reference in <<Draft Technical Report on C++ Library Extensions>>, P19)
void swap(MyString& a, MyString& b);

