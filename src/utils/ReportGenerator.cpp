#include "utils/ReportGenerator.h"
#include "data/Database.h"
#include <iostream>

void ReportGenerator::generateInventoryReport() {
    auto cycles = Database::getInstance().getAllCycles();
    std::cout << "\n--- Cycle Inventory Report ---\n";
    std::cout << "ID\tType\tRented\tMaintenance\tCondition\n";
    for (const auto& cycle : cycles) {
        std::cout << cycle.id << "\t"
            << cycle.type << "\t"
            << (cycle.isRented ? "Yes" : "No") << "\t"
            << (cycle.needsMaintenance ? "Yes" : "No") << "\t"
            << cycle.condition << "\n";
    }
}

void ReportGenerator::generateTransactionReport() {
    auto transactions = Database::getInstance().getAllRentalTransactions();
    std::cout << "\n--- Rental Transaction Report ---\n";
    std::cout << "TransID\tCustID\tCycleID\tStartTime\t\tEndTime\t\tHours\tCost\n";
    for (const auto& trans : transactions) {
        std::cout << trans.transactionId << "\t"
            << trans.customerId << "\t"
            << trans.cycleId << "\t"
            << trans.startTime << "\t"
            << trans.endTime << "\t"
            << trans.hoursRented << "\t"
            << trans.cost << "\n";
    }
}
