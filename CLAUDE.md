# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OOP Semester Project — Electricity Bill Predictor (WAPDA Pakistan domestic tariff).
Two fully independent parts that produce identical calculation results for the same inputs.

## Part 1: C++ Console Application

### Build command
```
g++ -o bill main.cpp Appliance.cpp HeavyAppliance.cpp LightAppliance.cpp House.cpp SlabTariff.cpp BillCalculator.cpp
```
Run: `./bill` (Linux/Mac) or `bill.exe` (Windows)

### Class hierarchy
- `Appliance` — abstract base: `getName()`, `getWattage()`, `getDailyHours()`, pure virtual `calculateMonthlyUnits()`
- `HeavyAppliance : Appliance` — 1.15× multiplier on base kWh (motor startup overhead)
- `LightAppliance : Appliance` — straightforward `(W × hrs × 30) / 1000` formula
- `House` — owns `vector<Appliance*>` (heap-allocated, freed in destructor); exposes `getTotalUnits()`, `getTopConsumer()`, `getAppliance(int)`
- `SlabTariff` — stateless; `calculateBill(units)` fills slab brackets sequentially; rates stored as `static const double`
- `BillCalculator` — holds `House&` + `SlabTariff`; `generateBill()` iterates via `house.getAppliance(i)`; `displaySavingTips()` uses `dynamic_cast<HeavyAppliance*>` to branch tips

### Input validation pattern (C++)
All user input goes through `readInt()`, `readDouble()`, or `readName()` helpers in `main.cpp`. They loop until valid, using `cin.fail()` + `cin.clear()` + `cin.ignore()` to flush bad state. Never call `cin >>` directly outside these helpers.

## Part 2: HTML/CSS/JS (index.html)

Single self-contained file — open directly in any browser, no server needed.

### Calculation parity with C++
- `monthlyUnits(app)`: heavy → `(W × hrs × 30 / 1000) × 1.15`, light → `(W × hrs × 30 / 1000)`
- `calculateWAPDABill(units)`: iterates `SLABS` array, filling each bracket; adds `FIXED_CHARGE = 75`
- These must stay identical to the C++ `calculateMonthlyUnits()` and `SlabTariff::calculateBill()`

### JS architecture
Global `appliances[]` array drives everything. `addAppliance()` validates → pushes → calls `renderApplianceList()`. `calculateBill()` validates owner name, maps units, renders results section. `escHtml()` is used on all user strings inserted via `innerHTML` (XSS prevention).

## WAPDA Tariff Constants (both parts must agree)

| Slab | Units | Rate (Rs./kWh) |
|------|-------|---------------|
| 1 | 1–100 | 5.79 |
| 2 | 101–200 | 10.06 |
| 3 | 201–300 | 14.84 |
| 4 | 301–700 | 19.55 |
| 5 | >700 | 26.40 |
| Fixed | — | 75.00/month |

If WAPDA revises these rates, update `SlabTariff.cpp` (static const doubles) **and** the `SLABS` array + `FIXED_CHARGE` in `index.html` together.

## Validation Rules (same in both parts)

| Field | Constraint |
|-------|-----------|
| Owner / Appliance name | Non-empty, non-whitespace-only, ≤50 chars, `[a-zA-Z0-9 \-]` only |
| Wattage | Numeric, 1–10000 W |
| Daily hours | Numeric, 0.1–24.0 |
| Appliance type | `heavy` or `light` only |
