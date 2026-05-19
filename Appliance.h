#ifndef APPLIANCE_H
#define APPLIANCE_H

#include <string>

// Abstract base class representing any electrical appliance
// All appliances share a name, wattage, and daily usage hours
class Appliance {
private:
    std::string applianceName; // Name/label of the appliance
    double wattage;            // Power consumption in watts
    double dailyHours;         // Hours used per day

public:
    // Constructor to initialize all core appliance attributes
    Appliance(const std::string& name, double watts, double hours);

    // Virtual destructor ensures derived class destructors are called correctly
    virtual ~Appliance() {}

    // Getters for private attributes
    std::string getName() const;
    double getWattage() const;
    double getDailyHours() const;

    // Pure virtual: every subclass must define how it calculates monthly units
    virtual double calculateMonthlyUnits() const = 0;

    // Virtual display method --- subclasses can override for custom output
    virtual void displayInfo() const;
};

#endif
