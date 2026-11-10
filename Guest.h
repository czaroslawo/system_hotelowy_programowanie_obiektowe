#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_GUEST_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_GUEST_H

#include "Person.h"

class Guest : public Person {
public:
    Guest(int id, std::string firstName, std::string lastName);
};

#endif //SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_GUEST_H
