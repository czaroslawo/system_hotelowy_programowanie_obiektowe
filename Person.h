#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_PERSON_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_PERSON_H

#include <string>

class Person {
private:
    int id;
    std::string firstName;
    std::string lastName;

public:
    Person(int id, std::string firstName, std::string lastName);
    virtual ~Person() = 0;
    int getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
};

#endif //SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_PERSON_H
