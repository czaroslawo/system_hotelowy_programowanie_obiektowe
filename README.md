# System hotelowy — dokumentacja

Projekt konsolowy w C++20 przedstawiający obsługę hotelu: dodawanie pokoi i recepcjonistów, wyszukiwanie wolnych pokoi, rezerwowanie pobytu, wybór usług, zameldowanie, wymeldowanie oraz wystawienie faktury. `main.cpp` wykonuje przykładowy scenariusz;

## 1. Budowanie i uruchamianie

Wymagania: CMake co najmniej 4.0 oraz kompilator z obsługą C++20 i użytych elementów biblioteki standardowej, w tym dat z `<chrono>`.

```sh
cmake -S . -B build
cmake --build build
./build/system_hotelowy_programowanie_obiektowe
```

Pliki `.h` zawierają deklaracje klas, a `.cpp` implementacje metod. `CMakeLists.txt` określa standard języka i pliki budowanego programu.

## 2. Klasy i ich odpowiedzialności

| Klasa / typ | Rola i najważniejsze dane |
| --- | --- |
| `Person` | Abstrakcyjna klasa osoby: ID, imię i nazwisko. Wymaga dodatniego ID oraz niepustego imienia i nazwiska. |
| `Guest` | Gość hotelowy; dziedziczy publicznie po `Person`. |
| `Receptionist` | Recepcjonista; dziedziczy publicznie po `Person`. Obecnie nie ma osobnych metod obsługi rezerwacji. |
| `Room` | Numer pokoju, pojemność i cena za noc. Numer i pojemność muszą być dodatnie, cena skończona i nieujemna. |
| `AdditionalService` | Nazwa i cena usługi. Prywatny konstruktor oraz `friend class HotelSystem` pozwalają systemowi tworzyć katalog usług. |
| `Reservation` | ID, referencje do gościa i pokoju, daty pobytu, czas utworzenia, status i wybrane usługi. Pilnuje zmian statusu oraz oblicza koszt. |
| `ReservationStatus` | `enum class` z wartościami `Confirmed`, `CheckedIn`, `CheckedOut`, `Cancelled`. |
| `Invoice` | Numer faktury, czas wystawienia, ID rezerwacji i kwota. `display()` wypisuje dane na konsolę. |
| `HotelSystem` | Przechowuje dane hotelu i koordynuje operacje, sprawdzając dostępność oraz przynależność przekazywanych obiektów. |

`Guest` i `Receptionist` korzystają z konstruktora `Person`. Deklaracja `virtual ~Person() = 0` czyni klasę bazową abstrakcyjną, a definicja `Person::~Person() = default` zapewnia destruktor wymagany przy niszczeniu klas pochodnych.

## 3. Główne operacje `HotelSystem`

| Metoda | Działanie |
| --- | --- |
| `addRoom(...)` | Sprawdza unikalność numeru, tworzy pokój i zwraca `Room&`. |
| `addReceptionist(...)` | Sprawdza unikalność ID recepcjonisty, tworzy go i zwraca referencję. |
| `getRooms()`, `getReservations()`, `getInvoices()`, `getReceptionists()`, `getServices()` | Udostępniają odpowiednie kontenery przez referencje do odczytu, bez ich kopiowania. |
| `findAvailableRooms(startsAt, endsAt)` | Zwraca `std::vector<Room*>` wskazujący pokoje dostępne w podanym terminie. |
| `reserve(guest, room, startsAt, endsAt)` | Sprawdza dostępność i przynależność pokoju, tworzy rezerwację `Confirmed` i zwraca `Reservation&`. |
| `addService(reservation, service)` | Sprawdza przynależność rezerwacji i usługi do hotelu, następnie dodaje usługę. |
| `cancelReservation(reservation)` | Anuluje potwierdzoną rezerwację. |
| `checkInGuest(reservation)` | Zmienia status potwierdzonej rezerwacji na `CheckedIn`. |
| `checkOutGuest(reservation)` | Zmienia `CheckedIn` na `CheckedOut` i zwraca całkowity koszt. |
| `issueInvoice(reservation)` | Wystawia jedną fakturę dla wymeldowanej rezerwacji i zwraca `Invoice&`. |

Prywatna metoda `validateReservation()` porównuje adresy obiektów, aby sprawdzić, czy otrzymana rezerwacja jest rzeczywiście przechowywana w tym hotelu. Sama zgodność ID nie wystarcza. ID rezerwacji i numery faktur są nadawane jako liczba istniejących elementów plus jeden.

## 4. Rezerwacje, terminy i koszt

Dozwolone przejścia statusów:

```text
Confirmed ──checkIn()──> CheckedIn ──checkOut()──> CheckedOut
    │
    └──cancel()──> Cancelled
```

Usługi można dodawać tylko w stanach `Confirmed` i `CheckedIn`. Powtórne dodanie usługi o tej samej nazwie jest zabronione. Fakturę można wystawić dopiero po wymeldowaniu i tylko raz dla danej rezerwacji.

Daty pobytu mają typ `std::chrono::sys_days`, czyli daty z dokładnością do dnia. Początek musi być wcześniejszy od końca. Termin jest traktowany jako przedział `[startsAt, endsAt)`: dzień wyjazdu nie jest kolejną płatną nocą, a nowy pobyt może zaczynać się w dniu zakończenia poprzedniego.

Dwa terminy nakładają się, gdy:

```cpp
startsAt < reservation.getEndsAt()
    && reservation.getStartsAt() < endsAt
```

Wyszukiwanie sprawdza rezerwacje tego samego obiektu pokoju. Rezerwacje anulowane i wymeldowane nie blokują dostępności.

Koszt jest obliczany przez `Reservation::calculateCost()`:

```text
koszt = liczba nocy × cena pokoju za noc + suma cen wybranych usług
```

Każda wybrana usługa jest naliczana raz na rezerwację, bez mnożenia przez liczbę nocy lub osób. Kwoty są przechowywane jako `double`; program nie implementuje podatków ani rozliczania płatności.

Katalog tworzony przez `HotelSystem`:

| Usługa | Cena (PLN) |
| --- | ---: |
| Breakfast | 40 |
| Parking | 30 |
| Spa | 120 |
| AirportTransfer | 100 |
| Laundry | 25 |
| Dinner | 70 |

## 5. Obiekty, referencje i kontenery

- **Pełne obiekty w `std::deque`** — hotel przechowuje pokoje, rezerwacje, faktury, recepcjonistów i katalog usług. Dodawanie na końcu `deque` nie unieważnia referencji do istniejących elementów, co jest potrzebne przy powiązaniach między obiektami.
- **Referencje `Room&` i `Guest&`** — rezerwacja odwołuje się do istniejącego pokoju i gościa zamiast tworzyć ich kopie. Gości tworzy kod wywołujący, np. `main.cpp`.
- **Wskaźniki w `std::vector<Room*>`** — wynik wyszukiwania zawiera adresy pokoi należących do hotelu. Powiększanie wektora przenosi wskaźniki, a nie same pokoje.
- **`std::reference_wrapper<const AdditionalService>`** — pozwala przechowywać odwołania do usług w wektorze. Zwykły `std::vector<T&>` jest niedozwolony. `std::cref(service)` tworzy takie opakowanie do odczytu, a `.get()` udostępnia wskazaną usługę.
- **Czas życia** — referencje i wskaźniki nie przedłużają życia obiektów. Hotel oraz gość muszą istnieć podczas korzystania z odwołujących się do nich rezerwacji. Wyników metod nie należy używać po zniszczeniu hotelu.
- **`= delete` w `HotelSystem`** — kopiowanie i przenoszenie systemu jest zabronione, co zapobiega tworzeniu kopii z powiązaniami nadal prowadzącymi do obiektów pierwotnego hotelu.

`deque` nie jest zawsze szybsze od `vector`. Tutaj wybór wynika przede wszystkim z potrzeby zachowania referencji do przechowywanych obiektów.

## 6. Najważniejsze zapisy C++ w projekcie

| Zapis | Znaczenie |
| --- | --- |
| `: name(std::move(name))` | Lista inicjalizacyjna konstruktora. Pole `name` jest inicjalizowane parametrem; `std::move` umożliwia przeniesienie jego zasobów zamiast kolejnego kopiowania napisu. |
| `const std::string& getName() const` | Zwraca istniejący napis bez kopiowania i bez możliwości zmiany przez wynik. Końcowe `const` oznacza metodę niezmieniającą zwykłych pól obiektu. |
| `double getPrice() const` | Zwraca kopię małej wartości liczbowej. |
| `auto` / `const auto&` | Typ jest ustalany przez kompilator na podstawie wyrażenia. Drugi zapis daje referencję do odczytu zamiast kopii. |
| `for (Room& room : rooms)` | W każdym obiegu udostępnia kolejny pokój przez referencję, bez kopiowania. |
| `for (const Room* r : available)` | Przechodzi po adresach pokoi. `r->getNumber()` wywołuje metodę wskazanego pokoju; przez `r` nie można go modyfikować. |
| `[number](const Room& room) { ... }` | Lambda, czyli mała funkcja. `[number]` przechwytuje wartość numeru, a parametr odnosi się do sprawdzanego pokoju. |
| `std::any_of` / `std::none_of` / `std::find` | Sprawdzają, czy istnieje element spełniający warunek, czy żaden go nie spełnia albo szukają konkretnej wartości. |
| `emplace_back(...)` / `back()` | Tworzy element bezpośrednio na końcu kontenera / zwraca referencję do ostatniego elementu. |
| `static_cast<int>(container.size())` | Jawnie konwertuje rozmiar kontenera na `int`; nie sprawdza przekroczenia zakresu tego typu. |
| `std::ostringstream` | Buduje tekst w pamięci za pomocą `<<`. Metoda `.str()` zwraca zbudowany napis. |
| `std::fixed << std::setprecision(2)` | Ustawia wyświetlanie liczb z dwiema cyframi po kropce; nie zmienia przechowywanej kwoty. |

## 7. Błędy i przykładowe wykonanie

Niepoprawne dane, np. zajęty numer pokoju, błędny termin lub obiekt spoza hotelu, powodują `std::invalid_argument`. Niedozwolone operacje, np. wymeldowanie przed zameldowaniem albo druga faktura, powodują `std::logic_error`. Wyjątki przerywają zwykłe wykonywanie operacji; przykładowy `main.cpp` nie zawiera ich obsługi przez `try` / `catch`.

Scenariusz w `main.cpp` tworzy pokoje 101 i 102, recepcjonistę oraz dwóch gości. Rezerwuje pokój 101 od 10 do 13 lipca 2026 r., dodaje śniadanie i parking, a później SPA. Osobna rezerwacja pokoju 102 zostaje anulowana. Pierwszy gość jest zameldowany, następnie wymeldowany i otrzymuje fakturę.

Kwota pierwszej rezerwacji wynika z działania:

```text
3 noce × 250 PLN + 40 PLN + 30 PLN + 120 PLN = 940 PLN
```

Faktura przechowuje wyliczoną kwotę oraz ID rezerwacji, a nie referencję do niej. Jej data jest wyświetlana w lokalnej strefie czasowej komputera. Zakończenie programu usuwa wszystkie dane przechowywane w pamięci.
