#pragma once
#ifndef EMPLOYEECONTROLLER_H
#define EMPLOYEECONTROLLER_H

#include <string>

class EmployeeController {
public:
    EmployeeController();
    void addEmployee(const std::string& name, const std::string& role);
};

#endif
