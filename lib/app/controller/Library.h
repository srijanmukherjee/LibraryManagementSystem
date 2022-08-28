#pragma once
#include <vector>
#include "../models/Book.h"
#include "../models/Member.h"
#include "../models/History.h"

using std::vector;

int CountAvailableQuantity(Book &book);
void LendBook(Book &book, Member &member, int issueFor);