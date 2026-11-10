#include "Reservation.h"

#include "AdditionalService.h"
#include "Room.h"

#include <algorithm>
#include <stdexcept>

Reservation::Reservation(int id, Guest& guest, Room& room,
                         std::chrono::sys_days startsAt,
                         std::chrono::sys_days endsAt,
                         std::chrono::system_clock::time_point createdAt,
                         ReservationStatus status)
    : id(id), guest(guest), room(room), startsAt(startsAt), endsAt(endsAt),
      createdAt(createdAt), status(status) {
    if (id <= 0 || startsAt >= endsAt) {
        throw std::invalid_argument("A reservation requires a positive ID and a valid date range.");
    }
}

int Reservation::getId() const {
    return id;
}

const Guest& Reservation::getGuest() const {
    return guest;
}

const Room& Reservation::getRoom() const {
    return room;
}

std::chrono::sys_days Reservation::getStartsAt() const {
    return startsAt;
}

std::chrono::sys_days Reservation::getEndsAt() const {
    return endsAt;
}

std::chrono::system_clock::time_point Reservation::getCreatedAt() const {
    return createdAt;
}

ReservationStatus Reservation::getStatus() const {
    return status;
}

const std::vector<std::reference_wrapper<const AdditionalService>>& Reservation::getServices() const {
    return services;
}

void Reservation::addService(const AdditionalService& service) {
    if (status != ReservationStatus::Confirmed && status != ReservationStatus::CheckedIn) {
        throw std::logic_error("Services cannot be added to a closed reservation.");
    }
    if (std::any_of(services.begin(), services.end(), [&service](const auto& existingService) {
        return existingService.get().getName() == service.getName();
    })) {
        throw std::logic_error("This service has already been added to the reservation.");
    }
    services.push_back(std::cref(service));
}

void Reservation::checkIn() {
    if (status != ReservationStatus::Confirmed) {
        throw std::logic_error("Only a confirmed reservation can be checked in.");
    }
    status = ReservationStatus::CheckedIn;
}

void Reservation::checkOut() {
    if (status != ReservationStatus::CheckedIn) {
        throw std::logic_error("Only a checked-in reservation can be checked out.");
    }
    status = ReservationStatus::CheckedOut;
}

double Reservation::calculateCost() const {
    double cost = (endsAt - startsAt).count() * room.getPricePerNight();
    for (const auto& service : services) {
        cost += service.get().getPrice();
    }
    return cost;
}

void Reservation::cancel() {
    if (status != ReservationStatus::Confirmed) {
        throw std::logic_error("Only a confirmed reservation can be cancelled.");
    }
    status = ReservationStatus::Cancelled;
}
