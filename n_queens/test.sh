#!/bin/bash

# Test Script for n_queens (Assignment‑Compatible)
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

PROGRAM="./n_queens"
test_count=0
passed=0
failed=0

# --------------------------------------------------
# Helper: Validate solution format and queen placement
# --------------------------------------------------
validate_solution() {
    local solution=$1
    local n=$2
    local positions=($solution)

    # Check correct number of columns
    if [ ${#positions[@]} -ne $n ]; then
        return 1
    fi

    # Check all values are integers within [0, n-1]
    for pos in "${positions[@]}"; do
        if ! [[ "$pos" =~ ^[0-9]+$ ]] || [ $pos -ge $n ] || [ $pos -lt 0 ]; then
            return 1
        fi
    done

    # Check rows and diagonals
    for ((i = 0; i < n; i++)); do
        for ((j = i + 1; j < n; j++)); do
            # Same row check
            if [ ${positions[$i]} -eq ${positions[$j]} ]; then
                return 1
            fi
            # Diagonal check (delta_row == delta_col)
            delta_row=$((positions[i] - positions[j]))
            delta_col=$((i - j))
            if [ ${delta_row#-} -eq ${delta_col#-} ]; then
                return 1
            fi
        done
    done

    return 0
}

# --------------------------------------------------
# Test Runner
# --------------------------------------------------
run_test() {
    ((test_count++))
    local description="$1"
    local command="$2"
    local expected_lines="$3"
    local validate="$4"

    printf "\nTest %02d: %s\n" $test_count "$description"
    printf "Command: %s\n" "$command"

    # Run command and capture output
    output=$(eval "$command" 2>&1)
    exit_code=$?
    line_count=$(echo "$output" | grep -v '^$' | wc -l | xargs)

    # Check line count
    success=1
    if [ "$line_count" -ne "$expected_lines" ]; then
        success=0
    fi

    # Validate solution format/placement if required
    if [ "$validate" = "true" ] && [ "$line_count" -gt 0 ]; then
        # Extract n from command (last argument)
        local n=$(echo "$command" | grep -oE '[0-9]+$')
        while IFS= read -r line; do
            if [ -n "$line" ]; then
                validate_solution "$line" "$n" || {
                    success=0
                    break
                }
            fi
        done <<<"$output"
    fi

    # Print result
    if [ $success -eq 1 ]; then
        echo -e "${GREEN}PASS${NC}"
        ((passed++))
    else
        echo -e "${RED}FAIL${NC}"
        echo "Expected lines: $expected_lines"
        echo "Actual lines:   $line_count"
        ((failed++))
    fi
}

# --------------------------------------------------
# Test Cases
# --------------------------------------------------

# Edge cases
run_test "n=0 (no solutions)" "$PROGRAM 0" 0 0
run_test "n=1 (single solution)" "$PROGRAM 1" 1 0

# No solutions
run_test "n=2" "$PROGRAM 2" 0 0
run_test "n=3" "$PROGRAM 3" 0 0

# Valid solutions
run_test "n=4" "$PROGRAM 4" 2 "true"
run_test "n=5" "$PROGRAM 5" 10 "true"
run_test "n=6" "$PROGRAM 6" 4 "true"
run_test "n=7" "$PROGRAM 7" 40 "true"
run_test "n=8" "$PROGRAM 8" 92 "true"

# Input validation (non-integer)
run_test "Non-integer input" "$PROGRAM abc" 0 0

# Add these test cases to the existing script
# --------------------------------------------------
# Test Cases (Enhanced)
# --------------------------------------------------

# Edge cases
run_test "n=0 (no solutions)" "$PROGRAM 0" 0 0
run_test "n=1 (single solution)" "$PROGRAM 1" 1 "true"

# No solutions
run_test "n=2" "$PROGRAM 2" 0 0
run_test "n=3" "$PROGRAM 3" 0 0

# Valid solutions (core)
run_test "n=4 (2 solutions)" "$PROGRAM 4" 2 "true"
run_test "n=5 (10 solutions)" "$PROGRAM 5" 10 "true"
run_test "n=6 (4 solutions)" "$PROGRAM 6" 4 "true"
run_test "n=7 (40 solutions)" "$PROGRAM 7" 40 "true"
run_test "n=8 (92 solutions)" "$PROGRAM 8" 92 "true"

# Input validation
run_test "Non-integer input" "$PROGRAM abc" 0 0
run_test "Negative input" "$PROGRAM -5" 0 0
run_test "Leading zero input" "$PROGRAM 04" 2 "true" # Tests n=4

# Harder Test Cases (Add to the end of the test cases section)
RUN_STRESS_TESTS=1 # Set to 0 to disable

if [ $RUN_STRESS_TESTS -eq 1 ]; then
    run_test "n=9 (352 solutions)" "$PROGRAM 9" 352 "true"
    run_test "n=10 (724 solutions)" "$PROGRAM 10" 724 "true"
    run_test "n=11 (2680 solutions)" "$PROGRAM 11" 2680 "true"
    run_test "n=12 (14200 solutions)" "$PROGRAM 12" 14200 "true"
    run_test "n=13 (73712 solutions)" "$PROGRAM 13" 73712 "true"
fi

# Format Validation Test (Add after helper function)
run_test "Output Format Check (n=4)" \
    "diff <($PROGRAM 4 | sed 's/ $//') <(echo '1 3 0 2'; echo '2 0 3 1')" \
    0 0
# Summary
echo -e "\nResults: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, Total: $test_count"
exit $failed
