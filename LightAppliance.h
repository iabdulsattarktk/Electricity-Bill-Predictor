#ifndef LIGHTAPPLIANCE_H
#define LIGHTAPPLIANCE_H

#include "Appliance.h"

// Derived class for low-power appliances (fans, bulbs, phone chargers, TVs, etc.)
// Uses straightforward kWh formula without any multiplier
class LightAppliance : public Appliance {
public:
    // Constructor delegates fully to base class
    LightAppliance(const std::string& name, double watts, double hours);

    // Calculate monthly kWh using simple formula: (W * hrs * 30) / 1000
    double calculateMonthlyUnits() const override;

    // Show light-appliance-specific display with type label
    void displayInfo() const override;
};

#endif
