#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_HOTEL_SYSTEM_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_HOTEL_SYSTEM_H

#include "AdditionalService.h"
#include "Invoice.h"
#include "Receptionist.h"
#include "Reservation.h"
#include "Room.h"

#include <deque>

class HotelSystem {
private:
    std::deque<Room> rooms;
    std::deque<Reservation> reservations;
    std::deque<Invoice> invoices;
    std::deque<Receptionist> receptionists;
    const std::deque<AdditionalService> services;

    void validateReservation(const Reservation& reservation) const;

public:
    HotelSystem();
    HotelSystem(const HotelSystem&) = delete;
    HotelSystem& operator=(const HotelSystem&) = delete;
    HotelSystem(HotelSystem&&) = delete;
    HotelSystem& operator=(HotelSystem&&) = delete;

    Room& addRoom(int number, int capacity, double pricePerNight);
    Receptionist& addReceptionist(int id, std::string firstName, std::string lastName);
    const std::deque<Room>& getRooms() const;
    const std::deque<Reservation>& getReservations() const;
    const std::deque<Invoice>& getInvoices() const;
    const std::deque<Receptionist>& getReceptionists() const;
    const std::deque<AdditionalService>& getServices() const;
    std::vector<Room*> findAvailableRooms(std::chrono::sys_days startsAt,
                                          std::chrono::sys_days endsAt);
    Reservation& reserve(Guest& guest, Room& room, std::chrono::sys_days startsAt,
                         std::chrono::sys_days endsAt);
    void addService(Reservation& reservation, const AdditionalService& service);
    void cancelReservation(Reservation& reservation);
    void checkInGuest(Reservation& reservation);
    Invoice& checkOutGuest(Reservation& reservation);
};

#endif
