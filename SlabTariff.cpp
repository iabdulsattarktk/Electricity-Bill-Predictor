#include "SlabTariff.h"
#include <iostream>
#include <iomanip>
#include <string>

// WAPDA domestic tariff rates (Rs. per kWh) --- update if WAPDA revises them
const double SlabTariff::RATE_SLAB1   =  5.79;
const double SlabTariff::RATE_SLAB2   = 10.06;
const double SlabTariff::RATE_SLAB3   = 14.84;
const double SlabTariff::RATE_SLAB4   = 19.55;
const double SlabTariff::RATE_SLAB5   = 26.40;
const double SlabTariff::FIXED_CHARGE = 75.00;

// Calculate bill by filling each slab bracket in order:
// Units up to the bracket ceiling are billed at that bracket's rate.
// Remaining units spill into the next bracket.
double SlabTariff::calculateBill(double totalUnits) const {
    double bill = FIXED_CHARGE; // Start with fixed monthly charge
    double remaining = totalUnits;

    if (remaining <= 0) return bill;

    // Slab 1: 1–100 units @ Rs. 5.79
    double slab1Units = (remaining > 100) ? 100 : remaining;
    bill += slab1Units * RATE_SLAB1;
    remaining -= slab1Units;

    // Slab 2: 101–200 units @ Rs. 10.06
    if (remaining > 0) {
        double slab2Units = (remaining > 100) ? 100 : remaining;
        bill += slab2Units * RATE_SLAB2;
        remaining -= slab2Units;
    }

    // Slab 3: 201–300 units @ Rs. 14.84
    if (remaining > 0) {
        double slab3Units = (remaining > 100) ? 100 : remaining;
        bill += slab3Units * RATE_SLAB3;
        remaining -= slab3Units;
    }

    // Slab 4: 301–700 units @ Rs. 19.55
    if (remaining > 0) {
        double slab4Units = (remaining > 400) ? 400 : remaining;
        bill += slab4Units * RATE_SLAB4;
        remaining -= slab4Units;
    }

    // Slab 5: above 700 units @ Rs. 26.40
    if (remaining > 0) {
        bill += remaining * RATE_SLAB5;
    }

    return bill;
}

// Print a formatted table of all slab brackets with per-unit rates
void SlabTariff::displaySlabs() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n  +-----------------------+-------------+\n"
              << "  |  WAPDA Slab Brackets  |  Rate (Rs.) |\n"
              << "  +-----------------------+-------------+\n"
              << "  |  1   - 100  units     |    5.79/kWh |\n"
              << "  |  101 - 200  units     |   10.06/kWh |\n"
              << "  |  201 - 300  units     |   14.84/kWh |\n"
              << "  |  301 - 700  units     |   19.55/kWh |\n"
              << "  |  Above 700  units     |   26.40/kWh |\n"
              << "  |  Fixed Charge         |   Rs. 75.00 |\n"
              << "  +-----------------------+-------------+\n";
}

// Return a human-readable slab name for the given consumption level
std::string SlabTariff::getApplicableSlab(double totalUnits) const {
    if (totalUnits <= 100)  return "Slab 1 (1-100 units @ Rs. 5.79/kWh)";
    if (totalUnits <= 200)  return "Slab 2 (101-200 units @ Rs. 10.06/kWh)";
    if (totalUnits <= 300)  return "Slab 3 (201-300 units @ Rs. 14.84/kWh)";
    if (totalUnits <= 700)  return "Slab 4 (301-700 units @ Rs. 19.55/kWh)";
    return "Slab 5 (Above 700 units @ Rs. 26.40/kWh)";
}
