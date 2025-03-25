#pragma once
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

// Represents an employee 
class Employee {
public:
    int id;
    std::string name;
    std::string role;

    
    Employee(int id, const std::string& name, const std::string& role);
};

#endif
