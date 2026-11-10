#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RESERVATION_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RESERVATION_H

#include <chrono>
#include <functional>
#include <vector>

#include "ReservationStatus.h"

class Guest;
class Room;
class AdditionalService;

class Reservation {
private:
    int id;
    Guest& guest;
    Room& room;
    std::chrono::sys_days startsAt;
    std::chrono::sys_days endsAt;
    std::chrono::system_clock::time_point createdAt;
    ReservationStatus status;
    std::vector<std::reference_wrapper<const AdditionalService>> services;

public:
    Reservation(int id, Guest& guest, Room& room,
                std::chrono::sys_days startsAt,
                std::chrono::sys_days endsAt,
                std::chrono::system_clock::time_point createdAt,
                ReservationStatus status);
    int getId() const;
    const Guest& getGuest() const;
    const Room& getRoom() const;
    std::chrono::sys_days getStartsAt() const;
    std::chrono::sys_days getEndsAt() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    ReservationStatus getStatus() const;
    const std::vector<std::reference_wrapper<const AdditionalService>>& getServices() const;
    void addService(const AdditionalService& service);
    double calculateCost() const;
    void checkIn();
    void checkOut();
    void cancel();
};

#endif //SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_RESERVATION_H
