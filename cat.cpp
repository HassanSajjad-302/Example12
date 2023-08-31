module;

#ifdef INCLUDE_IOSTREAM
#include <iostream>
#endif

export module cat;

import std;

export void func()
{
    if (__cplusplus == 202002L)
    {
        std::cout << "Cat.cpp compiled with /std:c++20 with dependency on std.cpp compiled with /std:c++20 as well\n";
    }
    else
    {
        std::cout
            << "Cat.cpp compiled with /std:c++latest with dependency on std.cpp compiled with /std:c++latest as well\n";
    }
}