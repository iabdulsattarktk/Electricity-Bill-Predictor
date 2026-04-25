#include "LightAppliance.h"
#include <iostream>
#include <iomanip>

LightAppliance::LightAppliance(const std::string& name, double watts, double hours)
    : Appliance(name, watts, hours) {}

// Monthly units for light appliances:
// Formula: (wattage in W * hours per day * 30 days) / 1000 = kilowatt-hours
// No multiplier — light appliances consume close to their rated wattage
double LightAppliance::calculateMonthlyUnits() const {
    return (getWattage() * getDailyHours() * 30.0) / 1000.0;
}

void LightAppliance::displayInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  [LIGHT] " << getName() << "\n"
              << "  Wattage   : " << getWattage() << " W\n"
              << "  Daily Use : " << getDailyHours() << " hrs\n"
              << "  Monthly   : " << calculateMonthlyUnits() << " kWh\n";
}
