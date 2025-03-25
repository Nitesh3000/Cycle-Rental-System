#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <sqlite3.h>
#include "models/Customer.h"
#include "models/Cycle.h"
#include "models/RentalTransaction.h"
#include "models/Employee.h"
#include "models/Payment.h"
#include <controllers/Employee.h>

// Database class encapsulates all database operations.
class Database {
private:
    sqlite3* db;
    Database();                
    void createTables();      

public:
    static Database& getInstance();
    ~Database();

   
    bool addCustomer(const Customer& customer);
    std::vector<Customer> getAllCustomers();
    bool getCustomerById(int customerId, Customer& customer);

  
    bool addCycle(const Cycle& cycle);
    std::vector<Cycle> getAllCycles();
    bool getCycleById(int cycleId, Cycle& cycle);
    bool updateCycleRentalStatus(int cycleId, bool isRented);

   
    bool addRentalTransaction(const RentalTransaction& transaction);
    std::vector<RentalTransaction> getAllRentalTransactions();
    bool getRentalTransactionById(int transactionId, RentalTransaction& transaction);

   
    bool isCycleCurrentlyRented(int cycleId);
    bool updateRentalTransaction(int transactionId, const std::string& endTime, int hoursRented, double cost);

    bool addEmployee(const Employee& employee);

    std::vector<Employee> getAllEmployees();

    bool addPayment(const Payment& payment);

    std::vector<Payment> getAllPayments();

   
};

#endif
