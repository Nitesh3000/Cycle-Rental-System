#pragma once
#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include <string>

class CustomerController {
public:
    CustomerController();
    int registerCustomer(const std::string& name, const std::string& contact, const std::string& license = "");
};

#endif
