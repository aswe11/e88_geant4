# Migration Plan and Workflow

This document outlines the workflow phases and provides a checklist for the migration process.

## Workflow Policy

### Phase 1 — Inspection
First inspect the repository and summarize findings. Before modifying files:
- Identify CCJ-specific components and required dependencies.
- Identify possible incompatibilities and runtime assumptions.
- **Note:** Do not modify files during the inspection phase.

### Phase 2 — Migration Plan
Create a migration strategy including:
- Required environment changes and dependency issues.
- Build-system and runtime issues.
- Proposed solutions and possible risks.
- Group changes by category.

### Phase 3 — Incremental Modification
Apply only the minimum necessary modifications required for KEKCC execution. After each modification:
- Explain what changed.
- Explain why it changed.
- Explain possible side effects.

---

## Migration Checklist

### Environment
- [ ] ROOT works
- [ ] Geant4 works
- [ ] CLHEP linked
- [ ] Required environment variables set

### Build
- [ ] Compile succeeds
- [ ] Shared libraries generated
- [ ] No missing symbols
- [ ] Executable runs

### Runtime
- [ ] Macro execution works
- [ ] Geometry loads
- [ ] Input ROOT files readable
- [ ] Detector initialization succeeds

### Physics Validation
- [ ] Event count consistent
- [ ] Spectra shape consistent
- [ ] Random seed behavior checked
- [ ] Reconstruction consistency checked
- [ ] Output ROOT structure verified
