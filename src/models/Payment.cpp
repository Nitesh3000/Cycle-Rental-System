#include "models/Payment.h"

Payment::Payment(int paymentId, int transactionId, double amount, const std::string& paymentDate)
    : paymentId(paymentId), transactionId(transactionId), amount(amount), paymentDate(paymentDate) {
}
