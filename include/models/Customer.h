#pragma once
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

// Represents a registered customer.
class Customer {
public:
    int id;
    std::string name;
    std::string contact;
    std::string license;

   
    Customer(int id, const std::string& name, const std::string& contact, const std::string& license = "");
};

#endif
