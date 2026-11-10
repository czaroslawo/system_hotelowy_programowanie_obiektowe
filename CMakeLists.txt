cmake_minimum_required(VERSION 4.0)
project(system_hotelowy_programowanie_obiektowe)

set(CMAKE_CXX_STANDARD 20)

add_executable(system_hotelowy_programowanie_obiektowe main.cpp
        Room.cpp
        Room.h
        AdditionalService.cpp
        AdditionalService.h
        Invoice.cpp
        Invoice.h
        Person.h
        Person.cpp
        Guest.h
        Guest.cpp
        Receptionist.h
        Receptionist.cpp
        Reservation.h
        Reservation.cpp
        ReservationStatus.h
        HotelSystem.h
        HotelSystem.cpp)
