#include "HeavyAppliance.h"
#include <iostream>
#include <iomanip>

HeavyAppliance::HeavyAppliance(const std::string& name, double watts, double hours, double peakHours)
    : Appliance(name, watts, hours), peakHoursUsage(peakHours) {}

// Monthly units for heavy appliances:
// Formula: (wattage * dailyHours * 30 / 1000) * 1.15
// The 1.15 multiplier reflects increased consumption from motor startup surges and heavy load
double HeavyAppliance::calculateMonthlyUnits() const {
    double baseUnits = (getWattage() * getDailyHours() * 30.0) / 1000.0;
    return baseUnits * 1.15; // 15% extra for heavy-load behaviour
}

// Return peak hours so file-save code can serialize it
double HeavyAppliance::getPeakHoursUsage() const { return peakHoursUsage; }

void HeavyAppliance::displayInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  [HEAVY] " << getName() << "\n"
              << "  Wattage    : " << getWattage() << " W\n"
              << "  Daily Use  : " << getDailyHours() << " hrs\n"
              << "  Peak Hours : " << peakHoursUsage << " hrs\n"
              << "  Monthly    : " << calculateMonthlyUnits() << " kWh (incl. 1.15x multiplier)\n";
}
