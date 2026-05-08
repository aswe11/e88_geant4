# KEKCC Setup Guide for E88 Simulation

This document explains how to set up and run the E88 simulation in the KEKCC environment.

## 1. Environment Setup

Before building or running the simulation, source the KEKCC setup script:

### For Bash users:
```bash
source KEKCC_setup.sh
```

### For Csh users:
```csh
source KEKCC_setup.csh
```

> [!IMPORTANT]
> The default root path is set to `/group/e16/local`. If your E16 installation is elsewhere, set the `E16SYS` environment variable before sourcing the script.

## 2. Configuration Files

For KEKCC, use the KEKCC-specific calibration configuration:
- Path: `param/E16CalibFiles-KEKCC.cfg`

This file contains paths adjusted for the KEKCC filesystem. Ensure that the database files (field maps, etc.) are available at the paths specified in this file.

## 3. Building

To build the project:
```bash
make clean
make
```

The makefile will automatically detect your `E16SYS` and `E16ANA_VERSION` settings from the environment.

## 4. Execution

To run the simulation with KEKCC parameters:
```bash
./E16G4-yok.stat ./macro/pion-simple.mac <input-file> <output-file>
```

> [!NOTE]
> If the simulation fails to find a file, check the paths in `param/E16CalibFiles-KEKCC.cfg`.
