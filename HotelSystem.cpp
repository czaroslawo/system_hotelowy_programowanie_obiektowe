#include "HotelSystem.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

HotelSystem::HotelSystem()
    : services{AdditionalService("Breakfast", 40.0),
               AdditionalService("Parking", 30.0),
               AdditionalService("Spa", 120.0),
               AdditionalService("AirportTransfer", 100.0),
               AdditionalService("Laundry", 25.0),
               AdditionalService("Dinner", 70.0)} {}

Room& HotelSystem::addRoom(int number, int capacity, double pricePerNight) {
    if (std::any_of(rooms.begin(), rooms.end(), [number](const Room& room) {
        return room.getNumber() == number;
    })) {
        throw std::invalid_argument("A room with this number already exists.");
    }
    rooms.emplace_back(number, capacity, pricePerNight);
    return rooms.back();
}

Receptionist& HotelSystem::addReceptionist(int id, std::string firstName, std::string lastName) {
    if (std::any_of(receptionists.begin(), receptionists.end(), [id](const Receptionist& receptionist) {
        return receptionist.getId() == id;
    })) {
        throw std::invalid_argument("A receptionist with this ID already exists.");
    }
    receptionists.emplace_back(id, std::move(firstName), std::move(lastName));
    return receptionists.back();
}

const std::deque<Room>& HotelSystem::getRooms() const {
    return rooms;
}

const std::deque<Reservation>& HotelSystem::getReservations() const {
    return reservations;
}

const std::deque<Invoice>& HotelSystem::getInvoices() const {
    return invoices;
}

const std::deque<Receptionist>& HotelSystem::getReceptionists() const {
    return receptionists;
}

const std::deque<AdditionalService>& HotelSystem::getServices() const {
    return services;
}

std::vector<Room*> HotelSystem::findAvailableRooms(std::chrono::sys_days startsAt,
                                                 std::chrono::sys_days endsAt) {
    if (startsAt >= endsAt) {
        throw std::invalid_argument("A stay must last at least one night.");
    }
    std::vector<Room*> availableRooms;
    for (Room& room : rooms) {
        const bool occupied = std::any_of(reservations.begin(), reservations.end(),
            [&room, startsAt, endsAt](const Reservation& reservation) {
                return &reservation.getRoom() == &room
                    && reservation.getStatus() != ReservationStatus::Cancelled
                    && startsAt < reservation.getEndsAt()
                    && reservation.getStartsAt() < endsAt;
            });
        if (!occupied) {
            availableRooms.push_back(&room);
        }
    }
    return availableRooms;
}

Reservation& HotelSystem::reserve(Guest& guest, Room& room, std::chrono::sys_days startsAt,
                                  std::chrono::sys_days endsAt) {
    const auto availableRooms = findAvailableRooms(startsAt, endsAt);
    if (std::find(availableRooms.begin(), availableRooms.end(), &room) == availableRooms.end()) {
        throw std::invalid_argument("The room does not belong to this hotel or is unavailable.");
    }
    const int id = static_cast<int>(reservations.size()) + 1;
    reservations.emplace_back(id, guest, room, startsAt, endsAt,
                              std::chrono::system_clock::now(), ReservationStatus::Confirmed);
    return reservations.back();
}

void HotelSystem::validateReservation(const Reservation& reservation) const {
    if (std::none_of(reservations.begin(), reservations.end(), [&reservation](const Reservation& existingReservation) {
        return &existingReservation == &reservation;
    })) {
        throw std::invalid_argument("The reservation does not belong to this hotel.");
    }
}

void HotelSystem::addService(Reservation& reservation, const AdditionalService& service) {
    validateReservation(reservation);
    if (std::none_of(services.begin(), services.end(), [&service](const AdditionalService& existingService) {
        return &existingService == &service;
    })) {
        throw std::invalid_argument("Select a service from this hotel's catalogue.");
    }
    reservation.addService(service);
}

void HotelSystem::cancelReservation(Reservation& reservation) {
    validateReservation(reservation);
    reservation.cancel();
}

void HotelSystem::checkInGuest(Reservation& reservation) {
    validateReservation(reservation);
    reservation.checkIn();
}

Invoice& HotelSystem::checkOutGuest(Reservation& reservation) {
    validateReservation(reservation);
    if (reservation.getStatus() != ReservationStatus::CheckedIn) {
        throw std::logic_error("Only a checked-in reservation can be checked out.");
    }
    const int number = static_cast<int>(invoices.size()) + 1;
    invoices.emplace_back(number, std::chrono::system_clock::now(),
                          reservation.getId(), reservation.calculateCost());
    reservation.checkOut();
    return invoices.back();
}
