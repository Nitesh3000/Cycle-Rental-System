#include "models/Customer.h"

Customer::Customer(int id, const std::string& name, const std::string& contact, const std::string& license)
    : id(id), name(name), contact(contact), license(license) {
}
