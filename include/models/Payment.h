#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

// Represents a payment record.
class Payment {
public:
    int paymentId;
    int transactionId;
    double amount;
    std::string paymentDate;

    Payment(int paymentId, int transactionId, double amount, const std::string& paymentDate);
};

#endif
