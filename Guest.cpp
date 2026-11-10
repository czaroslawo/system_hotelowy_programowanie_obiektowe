#include "Guest.h"

#include <utility>

Guest::Guest(int id, std::string firstName, std::string lastName)
    : Person(id, std::move(firstName), std::move(lastName)) {}
