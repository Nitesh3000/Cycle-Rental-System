#pragma once
#ifndef RENTALTRANSACTION_H
#define RENTALTRANSACTION_H

#include <string>

// Represents a rental transaction.
class RentalTransaction {
public:
    int transactionId;
    int customerId;
    int cycleId;
    std::string startTime;
    std::string endTime;        // Remains empty until the cycle is returned.
    int hoursRented;
    double cost;

    RentalTransaction(int transactionId, int customerId, int cycleId, const std::string& startTime);
    void completeTransaction(const std::string& endTime, int hours, double cost);
};

#endif 