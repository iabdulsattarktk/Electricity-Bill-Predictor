#ifndef HOUSE_H
#define HOUSE_H

#include "Appliance.h"
#include <vector>
#include <string>

// Represents a household that owns a collection of appliances
// Manages all appliances via base-class pointers (polymorphism)
class House {
private:
    std::string ownerName;               // Name of the electricity consumer
    std::vector<Appliance*> applianceList; // Pointers allow polymorphic dispatch

public:
    // Constructor sets owner name
    House(const std::string& owner);

    // Destructor frees all dynamically allocated Appliance objects
    ~House();

    // Add a heap-allocated appliance to this house
    void addAppliance(Appliance* appliance);

    // Sum calculateMonthlyUnits() across all appliances
    double getTotalUnits() const;

    // Return pointer to the appliance that consumes the most units
    Appliance* getTopConsumer() const;

    // Print details of every appliance in the list
    void displayAllAppliances() const;

    // Return the number of appliances added
    int getApplianceCount() const;

    // Access individual appliance by index (for bill breakdown)
    Appliance* getAppliance(int index) const;

    // Getter for owner name
    std::string getOwnerName() const;
};

#endif
