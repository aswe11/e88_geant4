# Migration Policy

This document defines the core principles and rules for migrating the E88 framework.

## Authority Priority

In case of conflict, follow this priority order:

1. **Preserve physics behavior** (Highest)
2. **Preserve original workflow**
3. **Maintain backward compatibility**
4. **Improve KEKCC portability**
5. **Code modernization** (Lowest)

If conflicts arise, follow the higher-priority rule.

---

## Core Principles

### Preserve Physics Behavior
- Preserve existing analysis logic.
- Preserve numerical behavior whenever possible.
- Do not modify physics parameters silently.
- If a change may affect physics outputs, explain it explicitly before applying changes.

### Minimize Modifications
- Avoid unnecessary refactoring.
- Avoid architectural rewrites.
- Prefer small incremental changes.
- Preserve original coding style.
- Do not reformat unrelated code.

### Maintain Backward Compatibility
- Keep CCJ-specific functionality whenever possible.
- Separate KEKCC-specific logic instead of overwriting CCJ logic.
- Prefer additive modifications over destructive modifications.

---

## Validation Baseline

The original CCJ output should be treated as the reference baseline. Whenever possible:
- Compare event counts.
- Compare spectra.
- Compare ROOT tree structures.
- Compare reconstruction behavior.

These comparisons should be performed between:
- Original CCJ execution.
- Migrated KEKCC execution.

---

## Forbidden Actions

Do NOT:
- Rewrite the framework architecture.
- Modernize unrelated code.
- Replace the build system unless absolutely required.
- Rename files/classes/functions unnecessarily.
- Remove legacy scripts or commented legacy code.
- Change physics constants silently.
- Apply cosmetic-only formatting changes.
- Delete functionality without explanation.

---

## Migration Policy

### Environment Isolation
Identify and isolate all CCJ-dependent assumptions:
- Absolute paths
- Environment variables
- ROOT, Geant4, and CLHEP versions
- Compiler and filesystem assumptions
- Shell setup scripts and batch system commands
- External library and calibration/database paths

### Setup Policy
- **Prefer:** Creating new KEKCC setup scripts and adding environment abstraction.
- **Avoid:** Overwriting original CCJ setup files or hard-coding KEKCC paths into shared logic.

---

## Code Style Policy
- Preserve existing style conventions (indentation, formatting).
- Avoid unnecessary modernization or introducing unrelated abstractions.

---

## Physics Safety Rule
Physics analysis behavior must remain unchanged unless explicitly requested. If a modification affects event selection, reconstruction, detector response, random-number behavior, fitting, numerical precision, or output spectra:
1. Explain the impact.
2. Document the modification.
3. Avoid silent changes.
