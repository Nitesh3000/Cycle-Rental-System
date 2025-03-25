#pragma once
#ifndef CYCLE_H
#define CYCLE_H

#include <string>

// Represents a cycle available for rent.
class Cycle {
public:
    int id;
    std::string type;           // e.g., "Standard", "Electric"
    bool isRented;              
    bool needsMaintenance;      
    std::string condition;      

   
    Cycle(int id, const std::string& type, const std::string& condition = "Good");
};

#endif
