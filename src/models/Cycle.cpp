#include "models/Cycle.h"

Cycle::Cycle(int id, const std::string& type, const std::string& condition)
    : id(id), type(type), isRented(false), needsMaintenance(false), condition(condition) {
}
