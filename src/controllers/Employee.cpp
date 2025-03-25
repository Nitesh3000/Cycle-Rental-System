#include "models/Employee.h"
#include <controllers/Employee.h>

Employee::Employee(int id, const std::string& name, const std::string& role)
    : id(id), name(name), role(role) {
}
