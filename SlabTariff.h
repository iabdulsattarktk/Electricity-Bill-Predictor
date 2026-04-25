#ifndef SLABTARIFF_H
#define SLABTARIFF_H

#include <string>

// Implements WAPDA Pakistan's slab-based electricity tariff system
// Each consumed unit is billed at the rate corresponding to its slab bracket
class SlabTariff {
private:
    // Rate constants (Rs./unit) matching WAPDA domestic tariff
    static const double RATE_SLAB1; // 1   – 100  units
    static const double RATE_SLAB2; // 101 – 200  units
    static const double RATE_SLAB3; // 201 – 300  units
    static const double RATE_SLAB4; // 301 – 700  units
    static const double RATE_SLAB5; // Above 700  units
    static const double FIXED_CHARGE; // Monthly fixed meter rent

public:
    // Compute the total bill by distributing units across applicable slabs
    double calculateBill(double totalUnits) const;

    // Print all slab brackets and their per-unit rates to the console
    void displaySlabs() const;

    // Return the name of the slab bracket that applies to totalUnits
    std::string getApplicableSlab(double totalUnits) const;
};

#endif
