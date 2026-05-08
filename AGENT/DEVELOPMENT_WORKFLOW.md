# Local-to-KEKCC Development Workflow

This document defines the process for developing and testing the E88 migration code.

## 1. Goal
Modify the code in the local environment and synchronize the working tree to KEKCC for build tests and execution.

---

## 2. Synchronization Target
- **Server:** `kekcc`
- **Path:** `/home/had/jws/e88/e88-geant4`

---

## 3. Workflow Steps

### Step 1: Local Development
- Apply modifications to the local `src/`, `include/`, or `makefile` as per the [MIGRATION_POLICY.md](file:///Users/JWS/Desktop/E88/E88_simulation/AGENT/MIGRATION_POLICY.md).
- Ensure changes are documented in [LOGS/MIGRATION_LOG.md](file:///Users/JWS/Desktop/E88/E88_simulation/LOGS/MIGRATION_LOG.md).

### Step 2: Synchronization
- Synchronize the working directory to KEKCC using `rsync` or `git`.
- **Note:** Exclude binaries, object files, and ROOT files (refer to [.gitignore](file:///Users/JWS/Desktop/E88/E88_simulation/.gitignore)).

### Step 3: Remote Build and Test
- SSH to KEKCC.
- Source the environment: `source KEKCC_setup.sh` (or `.csh`).
- Run the build: `make clean; make`.
- Execute tests and compare results with the CCJ baseline.

---

## 5. Role Separation

### User
The user will:
- Run synchronization when needed.
- Compile and test on KEKCC.
- Provide build/runtime logs.
- Decide when to commit or push.

### Agent
The agent will:
- Modify files locally.
- Analyze KEKCC build/runtime logs.
- Propose minimal fixes.
- Update migration documents.
- Prepare rsync/git commands when requested.

> [!CAUTION]
> The agent must not assume a fix is valid until KEKCC build/test results are provided by the user.

---

## 6. Build Responsibility and Agent Limitations

Compilation and execution on KEKCC are performed manually by the user.

### Agent Restrictions
The agent must **NOT**:
- Automatically SSH into KEKCC.
- Run remote builds.
- Execute remote jobs or submit batch jobs.
- Overwrite remote files without explicit instruction.

### Agent Scope
The agent should **ONLY**:
- Prepare local modifications.
- Suggest synchronization commands.
- Analyze user-provided logs.
- Propose fixes based on local analysis.
