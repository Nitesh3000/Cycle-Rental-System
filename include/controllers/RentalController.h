#pragma once
#ifndef RENTALCONTROLLER_H
#define RENTALCONTROLLER_H

#include <string>

class RentalController {
public:
    RentalController();
    int rentCycle(int customerId, int cycleId, const std::string& startTime);
    void returnCycle(int transactionId, const std::string& endTime, int hoursRented);
};

#endif