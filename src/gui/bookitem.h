#pragma once
#include <string>

struct BookItem
{
    int id;
    int count;
    int available;
    std::string code;
    std::string name;
    std::string author;
    std::string publisher;
};