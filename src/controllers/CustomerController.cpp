#include "controllers/CustomerController.h"
#include "data/Database.h"
#include "models/Customer.h"
#include "utils/Logger.h"
#include <iostream>

CustomerController::CustomerController() {}

int CustomerController::registerCustomer(const std::string& name, const std::string& contact, const std::string& license) {
    if (name.empty() || contact.empty()) {
        std::cerr << "Error: Name and contact cannot be empty.\n";
        return -1;
    }
    Database& db = Database::getInstance();
    auto customers = db.getAllCustomers();
    int newId = static_cast<int>(customers.size()) + 1;
    Customer newCustomer(newId, name, contact, license);
    if (db.addCustomer(newCustomer)) {
        std::cout << "Customer registered: " << name << " (ID: " << newId << ")\n";
        return newId;
    }
    else {
        std::cerr << "Failed to register customer.\n";
        return -1;
    }
}
