#include "AdditionalService.h"

#include <utility>

AdditionalService::AdditionalService(std::string name, double price)
    : name(std::move(name)), price(price) {}

const std::string& AdditionalService::getName() const {
    return name;
}

double AdditionalService::getPrice() const {
    return price;
}
