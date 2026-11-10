#include "Invoice.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

Invoice::Invoice(int number, std::chrono::system_clock::time_point issuedAt,
                 int reservationId, double amount)
    : number(number), issuedAt(issuedAt), reservationId(reservationId), amount(amount) {
    if (number <= 0 || reservationId <= 0 || !std::isfinite(amount) || amount < 0) {
        throw std::invalid_argument("Invalid invoice number, reservation ID or amount.");
    }
}

int Invoice::getNumber() const {
    return number;
}

std::chrono::system_clock::time_point Invoice::getIssuedAt() const {
    return issuedAt;
}

int Invoice::getReservationId() const {
    return reservationId;
}

double Invoice::getAmount() const {
    return amount;
}

void Invoice::display() const {
    const std::time_t issuedTime = std::chrono::system_clock::to_time_t(issuedAt);
    std::ostringstream output;
    output << "Invoice #" << number << '\n'
           << "Issued at: " << std::put_time(std::localtime(&issuedTime), "%Y-%m-%d %H:%M:%S") << '\n'
           << "Reservation ID: " << reservationId << '\n'
           << "Amount: " << std::fixed << std::setprecision(2) << amount << " PLN\n";
    std::cout << output.str();
}
