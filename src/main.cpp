#include <iostream>
#include <limits>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "data/Database.h"
#include "controllers/CustomerController.h"
#include "controllers/EmployeeController.h"
#include "controllers/RentalController.h"
#include "controllers/MaintenanceController.h"
#include "utils/ReportGenerator.h"
#include "models/Cycle.h"

using namespace std;

// Helper: Get current time as "YYYY-MM-DD HH:MM:SS"
string getCurrentTime() {
    time_t now = time(nullptr);
    tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return string(buffer);
}

void showMenu() {
    cout << "\n=== Cycle Rental System Menu ===\n";
    cout << "1. Register Customer\n";
    cout << "2. Add Cycle\n";
    cout << "3. Rent a Cycle\n";
    cout << "4. Return a Cycle\n";
    cout << "5. Add Employee\n";
    cout << "6. Mark Cycle for Maintenance\n";
    cout << "7. Complete Maintenance\n";
    cout << "8. Generate Inventory Report\n";
    cout << "9. Generate Rental Transaction Report\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    CustomerController custController;
    EmployeeController empController;
    RentalController rentalController;
    MaintenanceController maintController;

    int choice = -1;
    while (true) {
        showMenu();
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        switch (choice) {
        case 1: {
            string name, contact, license;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter customer name: ";
            getline(cin, name);
            cout << "Enter customer contact: ";
            getline(cin, contact);
            cout << "Enter customer license (optional): ";
            getline(cin, license);
            custController.registerCustomer(name, contact, license);
            break;
        }
        case 2: {
            int id;
            string type, condition;
            cout << "Enter cycle ID: ";
            cin >> id;
            if (id <= 0) {
                cout << "Invalid cycle ID. Must be positive.\n";
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter cycle type (e.g., Standard, Electric): ";
            getline(cin, type);
            cout << "Enter cycle condition (e.g., Good, Fair, Poor): ";
            getline(cin, condition);
            if (type.empty() || condition.empty()) {
                cout << "Cycle type and condition must not be empty.\n";
                break;
            }
            Cycle cycle(id, type, condition);
            Database& db = Database::getInstance();
            if (db.addCycle(cycle))
                cout << "Cycle added successfully.\n";
            else
                cout << "Failed to add cycle.\n";
            break;
        }
        case 3: {
            int customerId, cycleId;
            string startTime;
            cout << "Enter customer ID: ";
            cin >> customerId;
            cout << "Enter cycle ID to rent: ";
            cin >> cycleId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter start time (YYYY-MM-DD HH:MM:SS) or press Enter for current time: ";
            getline(cin, startTime);
            if (startTime.empty()) {
                startTime = getCurrentTime();
            }
            rentalController.rentCycle(customerId, cycleId, startTime);
            break;
        }
        case 4: {
            int transactionId, hours;
            string endTime;
            cout << "Enter transaction ID: ";
            cin >> transactionId;
            cout << "Enter number of hours: ";
            cin >> hours;
            if (hours < 0) {
                cout << "Invalid hours. Must not be negative.\n";
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter end time (YYYY-MM-DD HH:MM:SS) or press Enter for current time: ";
            getline(cin, endTime);
            if (endTime.empty()) {
                endTime = getCurrentTime();
            }
            rentalController.returnCycle(transactionId, endTime, hours);
            break;
        }
        case 5: {
            string name, role;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter employee name: ";
            getline(cin, name);
            cout << "Enter employee role: ";
            getline(cin, role);
            empController.addEmployee(name, role);
            break;
        }
        case 6: {
            int cycleId;
            cout << "Enter cycle ID to mark for maintenance: ";
            cin >> cycleId;
            maintController.markCycleForMaintenance(cycleId);
            break;
        }
        case 7: {
            int cycleId;
            cout << "Enter cycle ID to complete maintenance: ";
            cin >> cycleId;
            maintController.completeMaintenance(cycleId);
            break;
        }
        case 8: {
            ReportGenerator::generateInventoryReport();
            break;
        }
        case 9: {
            ReportGenerator::generateTransactionReport();
            break;
        }
        case 0:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
