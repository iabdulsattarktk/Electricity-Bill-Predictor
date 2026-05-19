#ifndef HEAVYAPPLIANCE_H
#define HEAVYAPPLIANCE_H

#include "Appliance.h"

// Derived class for high-power appliances (AC, fridge, washing machine, etc.)
// Applies a 1.15x multiplier to account for motor startup and heavy load behaviour
class HeavyAppliance : public Appliance {
private:
    double peakHoursUsage; // Extra hours the appliance runs during peak load periods

public:
    // Constructor --- also accepts peak hours on top of daily hours
    HeavyAppliance(const std::string& name, double watts, double hours, double peakHours = 0.0);

    // Calculate monthly kWh with 1.15x heavy-load multiplier
    double calculateMonthlyUnits() const override;

    // Show heavy-appliance-specific details including peak hours and type label
    void displayInfo() const override;

    // Getter for peak hours --- needed when saving data to file
    double getPeakHoursUsage() const;
};

#endif
