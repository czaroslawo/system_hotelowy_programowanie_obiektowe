#include <chrono>
#include <iomanip>
#include <iostream>

#include "Guest.h"
#include "HotelSystem.h"

using namespace std::chrono;

int main() {
    HotelSystem hotel;

    Room& room101 = hotel.addRoom(101, 2, 250.0);
    Room& room102 = hotel.addRoom(102, 4, 420.0);
    Receptionist& rec = hotel.addReceptionist(1, "Anna", "Nowak");

    Guest guest1(1, "Jan", "Kowalski");
    Guest guest2(2, "Marek", "Zielinski");

    const sys_days checkInDate = year{2026} / July / 10;
    const sys_days checkOutDate = year{2026} / July / 13;

    std::cout << "=== 1. SPRAWDZENIE DOSTEPNOSCI POKOJU ===\n";
    auto available = hotel.findAvailableRooms(checkInDate, checkOutDate);
    for (const Room* r : available) {
        std::cout << "Pokoj nr " << r->getNumber()
                  << " | Miejsca: " << r->getCapacity()
                  << " | Cena/dobe: " << r->getPricePerNight() << " PLN\n";
    }

    std::cout << "\n=== 2. REZERWACJA POKOJU + 3. WYKUP USLUG ===\n";
    Reservation& res1 = hotel.reserve(guest1, room101, checkInDate, checkOutDate);
    std::cout << "Utworzono rezerwacje #" << res1.getId()
              << " dla: " << guest1.getFirstName() << " " << guest1.getLastName() << "\n";

    const auto& catalogue = hotel.getServices();
    hotel.addService(res1, catalogue[0]);
    hotel.addService(res1, catalogue[1]);

    std::cout << "Szacowany koszt po dodaniu uslug: "
              << std::fixed << std::setprecision(2) << res1.calculateCost() << " PLN\n";

    std::cout << "\n=== 4. ANULOWANIE REZERWACJI ===\n";
    Reservation& res2 = hotel.reserve(guest2, room102, checkInDate, checkOutDate);
    std::cout << "Utworzono rezerwacje #" << res2.getId() << " dla pokoju " << room102.getNumber() << "\n";
    hotel.cancelReservation(res2);
    std::cout << "Rezerwacja #" << res2.getId() << " zostala anulowana. "
              << "Wolnych pokoi w tym terminie: "
              << hotel.findAvailableRooms(checkInDate, checkOutDate).size() << "\n";

    std::cout << "\n=== 6. ZAMELDOWANIE GOSCIA (Recepcjonista: " << rec.getFirstName() << ") ===\n";
    hotel.checkInGuest(res1);
    std::cout << "Gosc zameldowany w pokoju " << res1.getRoom().getNumber() << ".\n";

    std::cout << "\n=== 5. DODANIE USLUG DODATKOWYCH W TRAKCIE POBYTU ===\n";
    hotel.addService(res1, catalogue[2]);
    std::cout << "Dodano usluge: " << catalogue[2].getName()
              << " (" << catalogue[2].getPrice() << " PLN)\n";

    std::cout << "\n=== 7. ODMELDOWANIE GOSCIA ===\n";
    double totalCost = hotel.checkOutGuest(res1);
    std::cout << "Gosc wymeldowany. Calkowity koszt pobytu: " << totalCost << " PLN\n";
    std::cout << "Wolnych pokoi po wymeldowaniu: "
              << hotel.findAvailableRooms(checkInDate, checkOutDate).size() << "\n";

    std::cout << "\n=== 8. WYSTAWIENIE FAKTURY ===\n";
    Invoice& invoice = hotel.issueInvoice(res1);
    invoice.display();

    return 0;
}