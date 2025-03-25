#include "controllers/MaintenanceController.h"
#include "data/Database.h"
#include "models/Cycle.h"
#include <iostream>

MaintenanceController::MaintenanceController() {}

void MaintenanceController::markCycleForMaintenance(int cycleId) {
    if (cycleId <= 0) {
        std::cerr << "Error: Cycle ID must be positive.\n";
        return;
    }
    auto cycles = Database::getInstance().getAllCycles();
    bool found = false;
    for (auto& cycle : cycles) {
        if (cycle.id == cycleId) {
            cycle.needsMaintenance = true;
            std::cout << "Cycle " << cycleId << " marked for maintenance.\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "Error: Cycle not found.\n";
    }
  
}

void MaintenanceController::completeMaintenance(int cycleId) {
    if (cycleId <= 0) {
        std::cerr << "Error: Cycle ID must be positive.\n";
        return;
    }
    auto cycles = Database::getInstance().getAllCycles();
    bool found = false;
    for (auto& cycle : cycles) {
        if (cycle.id == cycleId) {
            cycle.needsMaintenance = false;
            std::cout << "Maintenance completed for Cycle " << cycleId << ".\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "Error: Cycle not found.\n";
    }
  
}
