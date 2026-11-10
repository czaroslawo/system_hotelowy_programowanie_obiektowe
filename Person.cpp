#include "Person.h"

#include <stdexcept>
#include <utility>

Person::Person(int id, std::string firstName, std::string lastName)
    : id(id), firstName(std::move(firstName)), lastName(std::move(lastName)) {
    if (id <= 0 || this->firstName.empty() || this->lastName.empty()) {
        throw std::invalid_argument("A person requires a positive ID and a full name.");
    }
}

Person::~Person() = default;

int Person::getId() const {
    return id;
}

const std::string& Person::getFirstName() const {
    return firstName;
}

const std::string& Person::getLastName() const {
    return lastName;
}
