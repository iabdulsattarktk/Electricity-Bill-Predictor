#ifndef BILLCALCULATOR_H
#define BILLCALCULATOR_H

#include "House.h"
#include "SlabTariff.h"

// Orchestrates House and SlabTariff to produce a complete bill report
// Combines unit calculation, tariff lookup, and consumer advice
class BillCalculator {
private:
    House& house;         // Reference to the house whose bill we are computing
    SlabTariff tariff;    // SlabTariff instance (stateless, so value is fine here)

public:
    // Constructor takes a House by reference — does not own the House
    BillCalculator(House& h);

    // Print the full bill: per-appliance breakdown, totals, slab, amount due
    void generateBill() const;

    // Print 2–3 energy-saving tips tailored to the top-consuming appliance
    void displaySavingTips() const;
};

#endif
