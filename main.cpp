#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>
#include "House.h"
#include "HeavyAppliance.h"
#include "LightAppliance.h"
#include "BillCalculator.h"

// File where all consumer data is stored between runs
const std::string DATA_FILE = "consumers.dat";

// ---------------------------------------------------------------
// Input validation helpers
// ---------------------------------------------------------------

// Read a valid integer within [minVal, maxVal]; re-prompt on bad input
int readInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input. Please enter a number between "
                      << minVal << " and " << maxVal << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

// Read a valid double within [minVal, maxVal]; re-prompt on bad input
double readDouble(const std::string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input. Please enter a number between "
                      << minVal << " and " << maxVal << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

// Read a validated name string:
//   - Not empty, not all spaces
//   - Max 50 characters
//   - Only letters, digits, spaces, and hyphens
std::string readName(const std::string& prompt, const std::string& fieldLabel) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name);

        bool allSpaces = true;
        for (char c : name) {
            if (c != ' ') { allSpaces = false; break; }
        }

        if (name.empty() || allSpaces) {
            std::cout << "  Invalid input. " << fieldLabel << " must not be empty.\n";
            continue;
        }

        if (name.length() > 50) {
            std::cout << "  Invalid input. " << fieldLabel << " must not exceed 50 characters.\n";
            continue;
        }

        bool valid = true;
        for (char c : name) {
            if (!isalpha(c) && !isdigit(c) && c != ' ' && c != '-') {
                valid = false;
                break;
            }
        }

        if (!valid) {
            std::cout << "  Invalid input. " << fieldLabel
                      << " may only contain letters, digits, spaces, and hyphens.\n";
            continue;
        }

        return name;
    }
}

// ---------------------------------------------------------------
// File I/O — Save & Load
// ---------------------------------------------------------------

// Save all consumers and their appliances to consumers.dat
// File format per line:
//   C|owner_name
//   H|appliance_name|wattage|hours|peakHours   (heavy)
//   L|appliance_name|wattage|hours             (light)
void saveAllConsumers(const std::vector<House*>& houses) {
    std::ofstream file(DATA_FILE);
    if (!file.is_open()) {
        std::cout << "  Warning: Could not open '" << DATA_FILE << "' for writing.\n";
        return;
    }

    for (const House* h : houses) {
        // Write consumer header line
        file << "C|" << h->getOwnerName() << "\n";

        // Write each appliance — use dynamic_cast to detect Heavy vs Light
        for (int i = 0; i < h->getApplianceCount(); i++) {
            Appliance* a = h->getAppliance(i);
            HeavyAppliance* heavy = dynamic_cast<HeavyAppliance*>(a);
            if (heavy) {
                file << "H|" << a->getName() << "|"
                     << a->getWattage() << "|"
                     << a->getDailyHours() << "|"
                     << heavy->getPeakHoursUsage() << "\n";
            } else {
                file << "L|" << a->getName() << "|"
                     << a->getWattage() << "|"
                     << a->getDailyHours() << "\n";
            }
        }
    }

    file.close();
    std::cout << "\n  Data saved to '" << DATA_FILE << "' — "
              << houses.size() << " consumer(s) stored.\n";
}

// Load all consumers from consumers.dat into a vector of House pointers
// Returns empty vector if file does not exist yet
std::vector<House*> loadAllConsumers() {
    std::vector<House*> houses;
    std::ifstream file(DATA_FILE);
    if (!file.is_open()) return houses; // First run — no file yet

    House* current = nullptr;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Split line by '|' separator
        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, '|')) parts.push_back(token);

        if (parts.empty()) continue;

        if (parts[0] == "C" && parts.size() >= 2) {
            // New consumer block
            current = new House(parts[1]);
            houses.push_back(current);

        } else if (parts[0] == "H" && parts.size() >= 5 && current) {
            // Heavy appliance
            try {
                current->addAppliance(new HeavyAppliance(
                    parts[1],
                    std::stod(parts[2]),
                    std::stod(parts[3]),
                    std::stod(parts[4])
                ));
            } catch (...) {
                // Skip malformed lines silently
            }

        } else if (parts[0] == "L" && parts.size() >= 4 && current) {
            // Light appliance
            try {
                current->addAppliance(new LightAppliance(
                    parts[1],
                    std::stod(parts[2]),
                    std::stod(parts[3])
                ));
            } catch (...) {
                // Skip malformed lines silently
            }
        }
    }

    file.close();
    return houses;
}

// ---------------------------------------------------------------
// Console UI helpers
// ---------------------------------------------------------------

// Print the main menu showing currently active consumer name
void printMainMenu(const std::string& ownerName) {
    std::cout << "\n"
              << "  +============================================+\n"
              << "  |   ELECTRICITY BILL PREDICTOR — WAPDA PK   |\n"
              << "  +============================================+\n"
              << "  |  Consumer : " << ownerName << "\n"
              << "  +--------------------------------------------+\n"
              << "  |  1. Add Appliance                          |\n"
              << "  |  2. View All Appliances                    |\n"
              << "  |  3. Generate Bill                          |\n"
              << "  |  4. View WAPDA Tariff Slabs                |\n"
              << "  |  5. Switch / Add Consumer                  |\n"
              << "  |  6. View All Saved Consumers               |\n"
              << "  |  7. Save & Exit                            |\n"
              << "  +============================================+\n";
}

// Print a numbered list of all saved consumers with their appliance count
void showConsumerList(const std::vector<House*>& houses) {
    std::cout << "\n  +--------------------------------------------+\n"
              << "  |         All Saved Consumers                |\n"
              << "  +--------------------------------------------+\n";
    if (houses.empty()) {
        std::cout << "  |  No consumers saved yet.                   |\n"
                  << "  +--------------------------------------------+\n";
        return;
    }
    for (size_t i = 0; i < houses.size(); i++) {
        std::cout << "  " << (i + 1) << ". "
                  << houses[i]->getOwnerName()
                  << " — " << houses[i]->getApplianceCount() << " appliance(s)\n";
    }
    std::cout << "  +--------------------------------------------+\n";
}

// Ask user to select an existing consumer or create a new one.
// Adds the new House* to allHouses if creating fresh.
House* selectOrCreateConsumer(std::vector<House*>& allHouses) {
    if (!allHouses.empty()) {
        showConsumerList(allHouses);

        std::string prompt = "  Enter 0 to add NEW consumer, or 1-"
                             + std::to_string(allHouses.size())
                             + " to load existing: ";
        int sel = readInt(prompt, 0, (int)allHouses.size());

        if (sel > 0) {
            std::cout << "  Loaded consumer: " << allHouses[sel - 1]->getOwnerName() << "\n";
            return allHouses[sel - 1];
        }
    }

    // Create a brand-new consumer
    std::string ownerName = readName("  Enter new consumer name: ", "Consumer name");

    // Do not add duplicate — reuse if name already exists
    for (House* h : allHouses) {
        if (h->getOwnerName() == ownerName) {
            std::cout << "  Consumer '" << ownerName << "' already exists. Loading it.\n";
            return h;
        }
    }

    House* newHouse = new House(ownerName);
    allHouses.push_back(newHouse);
    std::cout << "  New consumer created: " << ownerName << "\n";
    return newHouse;
}

// ---------------------------------------------------------------
// Main entry point
// ---------------------------------------------------------------

int main() {
    std::cout << "\n  Welcome to the Electricity Bill Predictor\n";
    std::cout << "  -----------------------------------------\n";

    // Load all previously saved consumers from file
    std::vector<House*> allHouses = loadAllConsumers();

    if (!allHouses.empty()) {
        std::cout << "  " << allHouses.size()
                  << " consumer(s) loaded from '" << DATA_FILE << "'.\n";
    } else {
        std::cout << "  No saved data found — starting fresh.\n";
    }

    // Let user pick or create the first consumer to work on
    House* currentHouse = selectOrCreateConsumer(allHouses);

    int choice;
    do {
        printMainMenu(currentHouse->getOwnerName());
        choice = readInt("  Enter choice (1-7): ", 1, 7);

        switch (choice) {

        case 1: {
            // --- Add Appliance ---
            std::string appName = readName("  Enter appliance name: ", "Appliance name");
            double watts = readDouble("  Enter wattage (1 - 10000 W): ", 1.0, 10000.0);
            double hours = readDouble("  Enter daily usage hours (0.1 - 24.0): ", 0.1, 24.0);
            int type = readInt(
                "  Appliance type?\n  1. Heavy (AC, fridge, motor)\n  2. Light (fan, bulb, TV)\n  Choice (1-2): ",
                1, 2);

            if (type == 1) {
                double peakHrs = readDouble("  Enter peak hours usage (0 - 24.0): ", 0.0, 24.0);
                currentHouse->addAppliance(new HeavyAppliance(appName, watts, hours, peakHrs));
            } else {
                currentHouse->addAppliance(new LightAppliance(appName, watts, hours));
            }
            std::cout << "\n  Appliance added successfully!\n";
            break;
        }

        case 2: {
            // --- View All Appliances ---
            std::cout << "\n  --- Appliances in " << currentHouse->getOwnerName() << "'s House ---\n\n";
            currentHouse->displayAllAppliances();
            break;
        }

        case 3: {
            // --- Generate Bill ---
            if (currentHouse->getApplianceCount() == 0) {
                std::cout << "\n  Please add at least one appliance before generating a bill.\n";
            } else {
                BillCalculator calc(*currentHouse);
                calc.generateBill();
            }
            break;
        }

        case 4: {
            // --- View Tariff Slabs ---
            SlabTariff t;
            t.displaySlabs();
            break;
        }

        case 5: {
            // --- Switch / Add Consumer ---
            // Auto-save before switching so no data is lost
            saveAllConsumers(allHouses);
            currentHouse = selectOrCreateConsumer(allHouses);
            break;
        }

        case 6: {
            // --- View All Saved Consumers ---
            showConsumerList(allHouses);
            break;
        }

        case 7: {
            // --- Save & Exit ---
            saveAllConsumers(allHouses);
            std::cout << "\n  Thank you for using the Electricity Bill Predictor. Goodbye!\n\n";
            break;
        }

        } // end switch

    } while (choice != 7);

    // Free all heap-allocated House objects
    for (House* h : allHouses) delete h;

    return 0;
}
