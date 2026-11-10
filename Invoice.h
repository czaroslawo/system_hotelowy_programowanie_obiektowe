#ifndef SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_INVOICE_H
#define SYSTEM_HOTELOWY_PROGRAMOWANIE_OBIEKTOWE_INVOICE_H

#include <chrono>

class Invoice {
private:
    int number;
    std::chrono::system_clock::time_point issuedAt;
    int reservationId;
    double amount;

public:
    Invoice(int number, std::chrono::system_clock::time_point issuedAt,
            int reservationId, double amount);
    int getNumber() const;
    std::chrono::system_clock::time_point getIssuedAt() const;
    int getReservationId() const;
    double getAmount() const;
    void display() const;
};

#endif
