#include "models/RentalTransaction.h"

RentalTransaction::RentalTransaction(int transactionId, int customerId, int cycleId, const std::string& startTime)
    : transactionId(transactionId), customerId(customerId), cycleId(cycleId), startTime(startTime),
    endTime(""), hoursRented(0), cost(0.0) {
}

void RentalTransaction::completeTransaction(const std::string& endTime, int hours, double cost) {
    this->endTime = endTime;
    this->hoursRented = hours;
    this->cost = cost;
}
