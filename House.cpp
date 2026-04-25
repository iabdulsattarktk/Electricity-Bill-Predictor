#include "House.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

House::House(const std::string& owner) : ownerName(owner) {}

// Free every appliance allocated on the heap to avoid memory leaks
House::~House() {
    for (Appliance* a : applianceList) {
        delete a;
    }
}

void House::addAppliance(Appliance* appliance) {
    applianceList.push_back(appliance);
}

// Accumulate monthly units from all appliances using polymorphic dispatch
double House::getTotalUnits() const {
    double total = 0.0;
    for (const Appliance* a : applianceList) {
        total += a->calculateMonthlyUnits();
    }
    return total;
}

// Scan all appliances and return the one with the highest monthly consumption
Appliance* House::getTopConsumer() const {
    if (applianceList.empty()) return nullptr;

    Appliance* top = applianceList[0];
    for (Appliance* a : applianceList) {
        if (a->calculateMonthlyUnits() > top->calculateMonthlyUnits()) {
            top = a;
        }
    }
    return top;
}

// Display every appliance's details by calling its virtual displayInfo()
void House::displayAllAppliances() const {
    if (applianceList.empty()) {
        std::cout << "  No appliances added yet.\n";
        return;
    }
    for (size_t i = 0; i < applianceList.size(); i++) {
        std::cout << "  --- Appliance #" << (i + 1) << " ---\n";
        applianceList[i]->displayInfo();
        std::cout << "\n";
    }
}

int House::getApplianceCount() const { return (int)applianceList.size(); }
std::string House::getOwnerName() const { return ownerName; }

// Return appliance at index, or nullptr if out of range
Appliance* House::getAppliance(int index) const {
    if (index < 0 || index >= (int)applianceList.size()) return nullptr;
    return applianceList[index];
}
