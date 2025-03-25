#pragma once
#ifndef MAINTENANCECONTROLLER_H
#define MAINTENANCECONTROLLER_H

class MaintenanceController {
public:
    MaintenanceController();
    void markCycleForMaintenance(int cycleId);
    void completeMaintenance(int cycleId);
};

#endif
