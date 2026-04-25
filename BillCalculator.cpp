#include "BillCalculator.h"
#include "HeavyAppliance.h"
#include <iostream>
#include <iomanip>
#include <string>

BillCalculator::BillCalculator(House& h) : house(h), tariff() {}

// Generate a complete formatted electricity bill showing:
//   - Per-appliance unit consumption and percentage share
//   - Total units, applicable WAPDA slab, and total amount due
//   - The top-consuming appliance highlighted
void BillCalculator::generateBill() const {
    double totalUnits = house.getTotalUnits();
    double totalBill  = tariff.calculateBill(totalUnits);
    Appliance* top    = house.getTopConsumer();
    int count         = house.getApplianceCount();

    std::cout << "\n"
              << "  ============================================================\n"
              << "          ELECTRICITY BILL — " << house.getOwnerName() << "\n"
              << "  ============================================================\n\n";

    std::cout << std::fixed << std::setprecision(2);

    // Per-appliance breakdown table header
    std::cout << "  " << std::left << std::setw(22) << "Appliance"
              << std::right << std::setw(12) << "Units(kWh)"
              << std::setw(10) << "% Share"
              << std::setw(16) << "Cost (Rs.)\n";
    std::cout << "  " << std::string(60, '-') << "\n";

    // Print one row per appliance with its kWh, share, and proportional cost
    for (int i = 0; i < count; i++) {
        Appliance* a = house.getAppliance(i);
        if (!a) continue;

        double units = a->calculateMonthlyUnits();
        double pct   = (totalUnits > 0) ? (units / totalUnits * 100.0) : 0.0;
        // Proportional cost = fraction of total bill attributable to this appliance
        double cost  = (totalUnits > 0) ? (units / totalUnits * totalBill) : 0.0;

        // Mark the top consumer with an asterisk
        std::string marker = (a == top) ? " *" : "  ";

        std::cout << "  " << std::left << std::setw(22) << (a->getName() + marker)
                  << std::right << std::setw(12) << units
                  << std::setw(9)  << pct << "%"
                  << std::setw(14) << cost << "\n";
    }

    std::cout << "  " << std::string(60, '-') << "\n";
    std::cout << "  " << std::left << std::setw(22) << "TOTAL"
              << std::right << std::setw(12) << totalUnits
              << std::setw(9) << "100.00%"
              << std::setw(14) << totalBill << "\n\n";

    std::cout << "  Applicable Slab      : " << tariff.getApplicableSlab(totalUnits) << "\n";
    std::cout << "  Fixed Monthly Charge : Rs. 75.00\n";
    std::cout << "  Estimated Bill       : Rs. " << totalBill << "\n";

    if (top) {
        std::cout << "\n  ** Top Consumer: " << top->getName()
                  << " — " << top->calculateMonthlyUnits() << " kWh **\n";
    }

    std::cout << "  ============================================================\n";

    tariff.displaySlabs();
    displaySavingTips();
}

// Provide saving tips based on the type of the top-consuming appliance:
//   HeavyAppliance -> AC / fridge / motor tips
//   LightAppliance -> lighting and general tips
void BillCalculator::displaySavingTips() const {
    Appliance* top = house.getTopConsumer();
    if (!top) return;

    std::cout << "\n  ---- Energy Saving Tips ----\n";

    // dynamic_cast succeeds only when top actually points to a HeavyAppliance
    if (dynamic_cast<HeavyAppliance*>(top)) {
        std::cout << "  1. Set your AC to 26 deg C — each degree lower adds ~6% to consumption.\n"
                  << "  2. Service heavy appliances (AC/fridge) every 6 months for peak efficiency.\n"
                  << "  3. Run heavy loads (washing machine, AC) during off-peak hours (11 PM - 7 AM).\n";
    } else {
        std::cout << "  1. Replace incandescent bulbs with LEDs — they use 75% less energy.\n"
                  << "  2. Turn off lights and fans when leaving a room.\n"
                  << "  3. Use natural light during daytime to reduce your lighting load.\n";
    }
    std::cout << "\n";
}
