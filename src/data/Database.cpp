#include "data/Database.h"
#include <iostream>
#include <string>

Database::Database() : db(nullptr) {
    int rc = sqlite3_open("cycle_rental.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        db = nullptr;
    }
    else {
        std::cout << "Opened database successfully.\n";
    }
    createTables();
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

void Database::createTables() {
    char* errMsg = nullptr;

    const char* customer_sql = "CREATE TABLE IF NOT EXISTS Customers ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "contact TEXT, "
        "license TEXT"
        ");";
    int rc = sqlite3_exec(db, customer_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error creating Customers table: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    const char* cycle_sql = "CREATE TABLE IF NOT EXISTS Cycles ("
        "id INTEGER PRIMARY KEY, "
        "type TEXT NOT NULL, "
        "isRented INTEGER, "
        "needsMaintenance INTEGER, "
        "condition TEXT"
        ");";
    rc = sqlite3_exec(db, cycle_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error creating Cycles table: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    const char* transaction_sql = "CREATE TABLE IF NOT EXISTS Transactions ("
        "transactionId INTEGER PRIMARY KEY, "
        "customerId INTEGER, "
        "cycleId INTEGER, "
        "startTime TEXT, "
        "endTime TEXT, "
        "hoursRented INTEGER, "
        "cost REAL"
        ");";
    rc = sqlite3_exec(db, transaction_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error creating Transactions table: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    const char* employee_sql = "CREATE TABLE IF NOT EXISTS Employees ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "role TEXT"
        ");";
    rc = sqlite3_exec(db, employee_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error creating Employees table: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    const char* payment_sql = "CREATE TABLE IF NOT EXISTS Payments ("
        "paymentId INTEGER PRIMARY KEY, "
        "transactionId INTEGER, "
        "amount REAL, "
        "paymentDate TEXT"
        ");";
    rc = sqlite3_exec(db, payment_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error creating Payments table: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

// --- Customer Operations ---
bool Database::addCustomer(const Customer& customer) {
    if (customer.id <= 0) {
        std::cerr << "Error: Customer ID must be positive.\n";
        return false;
    }
    const char* sql = "INSERT INTO Customers (id, name, contact, license) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (addCustomer): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, customer.id);
    sqlite3_bind_text(stmt, 2, customer.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, customer.contact.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, customer.license.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed (addCustomer): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<Customer> Database::getAllCustomers() {
    std::vector<Customer> customers;
    const char* sql = "SELECT id, name, contact, license FROM Customers;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getAllCustomers): " << sqlite3_errmsg(db) << "\n";
        return customers;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string contact = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string license = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        customers.push_back(Customer(id, name, contact, license));
    }
    sqlite3_finalize(stmt);
    return customers;
}

bool Database::getCustomerById(int customerId, Customer& customer) {
    if (customerId <= 0) return false;
    const char* sql = "SELECT id, name, contact, license FROM Customers WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getCustomerById): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, customerId);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string contact = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string license = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        customer = Customer(id, name, contact, license);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

// --- Cycle Operations ---
bool Database::addCycle(const Cycle& cycle) {
    if (cycle.id <= 0) {
        std::cerr << "Error: Cycle ID must be positive.\n";
        return false;
    }
    const char* sql = "INSERT INTO Cycles (id, type, isRented, needsMaintenance, condition) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (addCycle): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, cycle.id);
    sqlite3_bind_text(stmt, 2, cycle.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, cycle.isRented ? 1 : 0);
    sqlite3_bind_int(stmt, 4, cycle.needsMaintenance ? 1 : 0);
    sqlite3_bind_text(stmt, 5, cycle.condition.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed (addCycle): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<Cycle> Database::getAllCycles() {
    std::vector<Cycle> cycles;
    const char* sql = "SELECT id, type, isRented, needsMaintenance, condition FROM Cycles;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getAllCycles): " << sqlite3_errmsg(db) << "\n";
        return cycles;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        bool isRented = sqlite3_column_int(stmt, 2) != 0;
        bool needsMaintenance = sqlite3_column_int(stmt, 3) != 0;
        std::string condition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        Cycle c(id, type, condition);
        c.isRented = isRented;
        c.needsMaintenance = needsMaintenance;
        cycles.push_back(c);
    }
    sqlite3_finalize(stmt);
    return cycles;
}

bool Database::getCycleById(int cycleId, Cycle& cycle) {
    if (cycleId <= 0) return false;
    const char* sql = "SELECT id, type, isRented, needsMaintenance, condition FROM Cycles WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getCycleById): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, cycleId);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        cycle.id = sqlite3_column_int(stmt, 0);
        cycle.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        cycle.isRented = sqlite3_column_int(stmt, 2) != 0;
        cycle.needsMaintenance = sqlite3_column_int(stmt, 3) != 0;
        cycle.condition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}
bool Database::updateRentalTransaction(int transactionId, const std::string& endTime, int hoursRented, double cost) {
    if (transactionId <= 0) return false;
    const char* sql = "UPDATE Transactions SET endTime = ?, hoursRented = ?, cost = ? WHERE transactionId = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (updateRentalTransaction): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_text(stmt, 1, endTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, hoursRented);
    sqlite3_bind_double(stmt, 3, cost);
    sqlite3_bind_int(stmt, 4, transactionId);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed (updateRentalTransaction): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
bool Database::updateCycleRentalStatus(int cycleId, bool isRented) {
    if (cycleId <= 0) return false;
    const char* sql = "UPDATE Cycles SET isRented = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (updateCycleRentalStatus): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, isRented ? 1 : 0);
    sqlite3_bind_int(stmt, 2, cycleId);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed (updateCycleRentalStatus): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

// --- RentalTransaction Operations ---
bool Database::addRentalTransaction(const RentalTransaction& transaction) {
    if (transaction.transactionId <= 0) {
        std::cerr << "Error: Transaction ID must be positive.\n";
        return false;
    }
    const char* sql = "INSERT INTO Transactions (transactionId, customerId, cycleId, startTime, endTime, hoursRented, cost) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (addRentalTransaction): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, transaction.transactionId);
    sqlite3_bind_int(stmt, 2, transaction.customerId);
    sqlite3_bind_int(stmt, 3, transaction.cycleId);
    sqlite3_bind_text(stmt, 4, transaction.startTime.c_str(), -1, SQLITE_STATIC);
    // Bind endTime. If empty, bind an empty string.
    const char* endTimeText = transaction.endTime.empty() ? "" : transaction.endTime.c_str();
    sqlite3_bind_text(stmt, 5, endTimeText, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, transaction.hoursRented);
    sqlite3_bind_double(stmt, 7, transaction.cost);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed (addRentalTransaction): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<RentalTransaction> Database::getAllRentalTransactions() {
    std::vector<RentalTransaction> transactions;
    const char* sql = "SELECT transactionId, customerId, cycleId, startTime, endTime, hoursRented, cost FROM Transactions;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getAllRentalTransactions): " << sqlite3_errmsg(db) << "\n";
        return transactions;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int transactionId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int cycleId = sqlite3_column_int(stmt, 2);
        std::string startTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        // For endTime, check for null pointer.
        const unsigned char* endText = sqlite3_column_text(stmt, 4);
        std::string endTime = endText ? reinterpret_cast<const char*>(endText) : "";
        int hoursRented = sqlite3_column_int(stmt, 5);
        double cost = sqlite3_column_double(stmt, 6);
        RentalTransaction trans(transactionId, customerId, cycleId, startTime);
        trans.completeTransaction(endTime, hoursRented, cost);
        transactions.push_back(trans);
    }
    sqlite3_finalize(stmt);
    return transactions;
}

bool Database::getRentalTransactionById(int transactionId, RentalTransaction& transaction) {
    if (transactionId <= 0) return false;
    const char* sql = "SELECT transactionId, customerId, cycleId, startTime, endTime, hoursRented, cost FROM Transactions WHERE transactionId = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getRentalTransactionById): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, transactionId);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int transId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int cycleId = sqlite3_column_int(stmt, 2);
        std::string startTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const unsigned char* endText = sqlite3_column_text(stmt, 4);
        std::string endTime = endText ? reinterpret_cast<const char*>(endText) : "";
        int hoursRented = sqlite3_column_int(stmt, 5);
        double cost = sqlite3_column_double(stmt, 6);
        transaction = RentalTransaction(transId, customerId, cycleId, startTime);
        transaction.completeTransaction(endTime, hoursRented, cost);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

bool Database::isCycleCurrentlyRented(int cycleId) {
    if (cycleId <= 0) return false;
    const char* sql = "SELECT COUNT(*) FROM Transactions WHERE cycleId = ? AND (endTime IS NULL OR endTime = '');";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (isCycleCurrentlyRented): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, cycleId);
    rc = sqlite3_step(stmt);
    bool active = false;
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        active = (count > 0);
    }
    sqlite3_finalize(stmt);
    return active;
}

// --- Employee Operations ---
bool Database::addEmployee(const Employee& employee) {
    if (employee.id <= 0) {
        std::cerr << "Error: Employee ID must be positive.\n";
        return false;
    }
    const char* sql = "INSERT INTO Employees (id, name, role) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (addEmployee): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, employee.id);
    sqlite3_bind_text(stmt, 2, employee.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, employee.role.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed (addEmployee): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<Employee> Database::getAllEmployees() {
    std::vector<Employee> employees;
    const char* sql = "SELECT id, name, role FROM Employees;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getAllEmployees): " << sqlite3_errmsg(db) << "\n";
        return employees;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        employees.push_back(Employee(id, name, role));
    }
    sqlite3_finalize(stmt);
    return employees;
}

// --- Payment Operations ---
bool Database::addPayment(const Payment& payment) {
    if (payment.paymentId <= 0) {
        std::cerr << "Error: Payment ID must be positive.\n";
        return false;
    }
    const char* sql = "INSERT INTO Payments (paymentId, transactionId, amount, paymentDate) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (addPayment): " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, payment.paymentId);
    sqlite3_bind_int(stmt, 2, payment.transactionId);
    sqlite3_bind_double(stmt, 3, payment.amount);
    sqlite3_bind_text(stmt, 4, payment.paymentDate.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed (addPayment): " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<Payment> Database::getAllPayments() {
    std::vector<Payment> payments;
    const char* sql = "SELECT paymentId, transactionId, amount, paymentDate FROM Payments;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed (getAllPayments): " << sqlite3_errmsg(db) << "\n";
        return payments;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int paymentId = sqlite3_column_int(stmt, 0);
        int transactionId = sqlite3_column_int(stmt, 1);
        double amount = sqlite3_column_double(stmt, 2);
        std::string paymentDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        payments.push_back(Payment(paymentId, transactionId, amount, paymentDate));
    }
    sqlite3_finalize(stmt);
    return payments;
}
