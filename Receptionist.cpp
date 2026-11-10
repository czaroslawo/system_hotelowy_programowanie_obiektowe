#include "Receptionist.h"

#include <utility>

Receptionist::Receptionist(int id, std::string firstName, std::string lastName)
    : Person(id, std::move(firstName), std::move(lastName)) {}
