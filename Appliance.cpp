#include "Appliance.h"
#include <iostream>
#include <iomanip>

// Initialize the appliance with name, wattage, and daily hours
Appliance::Appliance(const std::string& name, double watts, double hours)
    : applianceName(name), wattage(watts), dailyHours(hours) {}

std::string Appliance::getName() const { return applianceName; }
double Appliance::getWattage() const { return wattage; }
double Appliance::getDailyHours() const { return dailyHours; }

// Default display --- subclasses override this for type-specific details
void Appliance::displayInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Appliance : " << applianceName << "\n"
              << "  Wattage   : " << wattage << " W\n"
              << "  Daily Use : " << dailyHours << " hrs\n";
}
