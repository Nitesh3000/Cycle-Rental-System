#include "controllers/RentalController.h"
#include "data/Database.h"
#include "models/RentalTransaction.h"
#include "models/Cycle.h"
#include "models/Customer.h"
#include "utils/Logger.h"
#include <iostream>

RentalController::RentalController() {}

int RentalController::rentCycle(int customerId, int cycleId, const std::string& startTime) {
    if (customerId <= 0 || cycleId <= 0) {
        std::cerr << "Error: Customer ID and Cycle ID must be positive.\n";
        return -1;
    }

    Database& db = Database::getInstance();

    // Check if the customer exists.
    Customer cust(0, "", "", "");
    if (!db.getCustomerById(customerId, cust)) {
        std::cerr << "Error: Customer ID " << customerId << " is not registered.\n";
        return -1;
    }

    // Check if the cycle exists and is available.
    Cycle cycle(0, "");
    if (!db.getCycleById(cycleId, cycle)) {
        std::cerr << "Error: Cycle " << cycleId << " not found.\n";
        return -1;
    }
    if (db.isCycleCurrentlyRented(cycleId)) {
        std::cerr << "Error: Cycle " << cycleId << " is currently rented.\n";
        return -1;
    }

    auto transactions = db.getAllRentalTransactions();
    int newTransId = static_cast<int>(transactions.size()) + 1;
    RentalTransaction newTrans(newTransId, customerId, cycleId, startTime);

    if (db.addRentalTransaction(newTrans)) {
        // Mark the cycle as rented.
        db.updateCycleRentalStatus(cycleId, true);
        std::cout << "Cycle " << cycleId << " rented to Customer " << customerId
            << ". Transaction ID: " << newTransId << "\n";
        return newTransId;
    }
    else {
        std::cerr << "Rental failed.\n";
        return -1;
    }
}

void RentalController::returnCycle(int transactionId, const std::string& endTime, int hoursRented) {
    if (transactionId <= 0) {
        std::cerr << "Error: Transaction ID must be positive.\n";
        return;
    }
    if (hoursRented < 0) {
        std::cerr << "Error: Hours rented cannot be negative.\n";
        return;
    }

    Database& db = Database::getInstance();
    RentalTransaction trans(0, 0, 0, "");
    if (!db.getRentalTransactionById(transactionId, trans)) {
        std::cerr << "Error: Transaction not found.\n";
        return;
    }

    // Calculate cost (for example, 100 per hour).
    double cost = hoursRented * 100;

    // Update the rental transaction record with the return details.
    if (!db.updateRentalTransaction(transactionId, endTime, hoursRented, cost)) {
        std::cerr << "Error: Failed to update transaction record.\n";
        return;
    }

    std::cout << "Cycle returned for Transaction " << transactionId
        << ". End time: " << endTime << ", Hours: " << hoursRented
        << ", Cost: $" << cost << "\n";
    // Mark the cycle as available.
    db.updateCycleRentalStatus(trans.cycleId, false);
}
