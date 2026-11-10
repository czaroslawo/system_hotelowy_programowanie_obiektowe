#include "Room.h"

#include <cmath>
#include <stdexcept>

Room::Room(int number, int capacity, double pricePerNight)
    : number(number), capacity(capacity), pricePerNight(pricePerNight) {
    if (number <= 0 || capacity <= 0 || !std::isfinite(pricePerNight) || pricePerNight < 0) {
        throw std::invalid_argument("Invalid room number, capacity or price.");
    }
}

int Room::getNumber() const {
    return number;
}

int Room::getCapacity() const {
    return capacity;
}

double Room::getPricePerNight() const {
    return pricePerNight;
}
