---
name: debug-gnl
description: Debug GNL tester failures one test at a time with structured output and diff review
---

# Debug GNL Tester Skill

This skill debugs GNL (Get Next Line) tester failures following a strict one-test-at-a-time framework. Never process multiple test failures in one iteration.

## Setup
- **Tester location:** `gnlTester/` directory
- **Run command:** `wsl -e bash -c "cd /home/pauloh8755/projects/common-core/get-next-line/gnlTester && make 2>&1"`
- **Source files:** `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`
- **Test input files:** `gnlTester/files/` directory

## One-Test-At-A-Time Framework

### Step 1: Run Tester and Capture Output
Execute the tester command and capture the full output. Identify the FIRST test that fails (appears after a prior `OK` or at start). Stop after identifying one failure.

### Step 2: Extract Test Metadata
For the failing test, extract and present to the user:
- **Test name:** (e.g., `files/nl`, `files/41_no_nl`)
- **BUFFER_SIZE:** (e.g., 1, 42, 10M)
- **Input file path:** (e.g., `gnlTester/files/nl`)
- **Expected behavior:** Based on test name (e.g., "read file with newline")
- **Actual error/output:** Valgrind error, invalid read/write, segfault, etc.
- **Error location:** File, function, line number from stack trace
- **Scenario phrase:** One sentence summarizing what went wrong (e.g., "Buffer overflow when reading line that contains newline")

### Step 3: Present to User (Before Touching Code)
Display extracted metadata to the user in a clear format:
```
## Test Failure Analysis

**Test:** [name]  
**BUFFER_SIZE:** [size]  
**Input:** [path or description]  
**Scenario:** [one-sentence phrase]  

**Error:** [Valgrind error or crash type]  
**Location:** `[file]:[line]` in function `[func]()`  
**Details:** [relevant stack trace or memory info]
```

Wait for user to acknowledge before proceeding to code analysis.

### Step 4: Locate Root Cause
- Read the relevant source file(s)
- Trace execution flow from error location backward
- Identify logic flaw, off-by-one error, buffer boundary issue, etc.
- Do NOT apply fixes yet

### Step 5: Propose Correction as Diff
Show the fix as a structured diff (old lines → new lines) WITHOUT applying it:
```
## Proposed Fix

**File:** [path]  
**Function:** [name]  
**Change:**
- [old code line(s)]
+ [new code line(s)]

**Reason:** [one-sentence explanation of why this fixes the issue]
```

### Step 6: Wait for Explicit Confirmation
Ask user to confirm the proposed diff. Options:
- ✅ **y:** Apply the fix and test
- ❌ **n:** Discard and propose alternative
- 🔧 **?:** Request modification to the diff before applying

Only apply changes after explicit user approval.

### Step 7: Test the Fix
After approval:
1. Apply the fix using Edit tool in manual mode (user sees diff)
2. Re-run tester from Step 1
3. Check if this test now passes
4. If PASS: Move to next failure
5. If FAIL: Return to Step 4 with new error data

### Step 8: Move to Next Test
Once current test passes, loop back to Step 1 for the next failing test. Never batch multiple test fixes.

## Key Rules
- **One test at a time:** Identify, analyze, fix one test before moving to next
- **No code changes without diff review:** All edits use Edit tool; user must approve diff
- **Metadata first:** Always extract and present test info before reading source code
- **Diff before apply:** Always show diff and wait for confirmation before Edit
- **No assumptions:** If test output is unclear, re-run or ask user for clarification
