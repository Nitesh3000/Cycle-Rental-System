#include "controllers/EmployeeController.h"
#include "data/Database.h"
#include "models/Employee.h"
#include "utils/Logger.h"
#include <iostream>

EmployeeController::EmployeeController() {}

void EmployeeController::addEmployee(const std::string& name, const std::string& role) {
    if (name.empty() || role.empty()) {
        std::cerr << "Error: Employee name and role cannot be empty.\n";
        return;
    }
    Database& db = Database::getInstance();
    auto employees = db.getAllEmployees();
    int newId = static_cast<int>(employees.size()) + 1;
    Employee newEmployee(newId, name, role);
    if (db.addEmployee(newEmployee)) {
        std::cout << "Employee added: " << name << " (ID: " << newId << ")\n";
    }
    else {
        std::cerr << "Failed to add employee.\n";
    }
}
