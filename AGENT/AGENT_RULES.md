# E88 CCJ → KEKCC Migration Rules

## Project Goal

Port the existing E88 simulation/analysis framework originally developed on the RIKEN CCJ environment to the KEKCC environment while preserving the original physics behavior and workflow.

The migration should prioritize:
- reproducibility
- backward compatibility
- minimal modification
- physics safety

---

# Navigation

Detailed policies and plans are organized into the following documents:

- [MIGRATION_POLICY.md](file:///Users/JWS/Desktop/E88/E88_simulation/AGENT/MIGRATION_POLICY.md): Core principles, forbidden actions, and migration policies.
- [MIGRATION_PLAN.md](file:///Users/JWS/Desktop/E88/E88_simulation/AGENT/MIGRATION_PLAN.md): Detailed workflow phases and migration checklist.
- [DEVELOPMENT_WORKFLOW.md](file:///Users/JWS/Desktop/E88/E88_simulation/AGENT/DEVELOPMENT_WORKFLOW.md): Local-to-KEKCC development and sync process.
- [../docs/REFERENCE_DOCS.md](file:///Users/JWS/Desktop/E88/E88_simulation/docs/REFERENCE_DOCS.md): Authoritative reference documents and required deliverables.

---

# Agent Execution Rules

When working on this repository, you MUST:

1. **Inspect before modifying**: Always analyze the current state before proposing changes.
2. **Explain before refactoring**: Provide clear rationale for any structural changes.
3. **Preserve behavior**: Ensure physics outputs remain consistent.
4. **Prefer minimal edits**: Choose the least intrusive path to achieve the goal.
5. **Document every change**: Update logs and setup guides for every significant modification.
6. **Role Separation**: Adhere to the roles defined in [DEVELOPMENT_WORKFLOW.md](file:///Users/JWS/Desktop/E88/E88_simulation/AGENT/DEVELOPMENT_WORKFLOW.md).

> [!CAUTION]
> The agent must not assume a fix is valid until KEKCC build/test results are provided by the user.

The goal is **portability**, not redesign.
