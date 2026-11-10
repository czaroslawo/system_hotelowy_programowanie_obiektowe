#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_ADDITIONAL_SERVICE_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_ADDITIONAL_SERVICE_H

#include <string>

class HotelSystem;

class AdditionalService {
    friend class HotelSystem;

private:
    std::string name;
    double price;

    AdditionalService(std::string name, double price);

public:
    const std::string& getName() const;
    double getPrice() const;
};

#endif
