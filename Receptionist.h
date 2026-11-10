#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RECEPTIONIST_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RECEPTIONIST_H

#include "Person.h"

class Receptionist : public Person {
public:
    Receptionist(int id, std::string firstName, std::string lastName);
};

#endif //SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RECEPTIONIST_H
