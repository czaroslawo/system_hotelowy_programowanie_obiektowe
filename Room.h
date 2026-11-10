#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_ROOM_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_ROOM_H

class Room {
private:
    int number;
    int capacity;
    double pricePerNight;

public:
    Room(int number, int capacity, double pricePerNight);
    int getNumber() const;
    int getCapacity() const;
    double getPricePerNight() const;
};

#endif
