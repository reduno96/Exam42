#!/bin/bash

# Test script for vbc program
# Usage: ./test_vbc.sh

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Check if valgrind is installed
if ! command -v valgrind &>/dev/null; then
    echo "valgrind is not installed. Please install it using: sudo apt-get install valgrind"
    exit 1
fi

# Compile program first
make vbc || {
    echo -e "${RED}Compilation failed${NC}"
    exit 1
}

declare -a tests=(
    # Valid expressions
    "(3+2)*4:20"
    "3+2*4:11"
    "((2+1)*3)+4*(2+(3+2)):37"
    "9:9"
    "0*5+3:3"
    "9*9+9:90"
    "(((5))):5"
    "((3+2)*(4+5)):45"
    "2+3*4:14"
    "2*3+4*5:26"
    "2*(3+4)*5:70"

    # Error cases
    "3+a:Unexpected token 'a'"
    "4^2:Unexpected token '^'"
    "(3+2:Unexpected end of input"
    "3+2):Unexpected token ')'"
    "3++4:Unexpected token '+'"
    "+3:Unexpected token '+'"
    "3*:Unexpected end of input"
    "::Unexpected token ':'"
    "(3+*2):Unexpected token '*'"
    "3+4a*5:Unexpected token 'a'"
)

pass_count=0
fail_count=0
leak_count=0

run_test() {
    local input="$1"
    local expected="$2"
    local is_error=false
    [[ $expected == Unexpected* ]] && is_error=true
    local test_passed=true
    local leak_detected=false

    # Run program with valgrind
    valgrind --leak-check=full --error-exitcode=100 \
        --log-file=valgrind.log \
        ./vbc "$input" >actual.out 2>actual.err
    exit_code=$?

    # Check for memory leaks
    if grep -q "definitely lost:" valgrind.log && ! grep -q "definitely lost: 0 bytes" valgrind.log; then
        echo -e "${YELLOW}LEAK${NC}: '$input' (memory leak detected)"
        leak_detected=true
        ((leak_count++))
    fi

    if $is_error; then
        # Check error message and exit code (ignore valgrind error code 100)
        if [ $exit_code -ne 1 ] && [ $exit_code -ne 100 ]; then
            echo -e "${RED}FAIL${NC}: '$input' (wrong exit code: $exit_code)"
            test_passed=false
        elif ! grep -qF "$expected" actual.err; then
            echo -e "${RED}FAIL${NC}: '$input' (wrong error message)"
            test_passed=false
        fi
    else
        # Check output and exit code (ignore valgrind error code 100)
        if [ $exit_code -ne 0 ] && [ $exit_code -ne 100 ]; then
            echo -e "${RED}FAIL${NC}: '$input' (unexpected error exit)"
            test_passed=false
        elif ! grep -qF "$expected" actual.out; then
            echo -e "${RED}FAIL${NC}: '$input' (wrong output)"
            test_passed=false
        fi
    fi

    if $test_passed && ! $leak_detected; then
        echo -e "${GREEN}PASS${NC}: '$input'"
        ((pass_count++))
        return 0
    elif $test_passed && $leak_detected; then
        # Test functionally passed but has memory leaks
        ((pass_count++))
        return 0
    else
        ((fail_count++))
        return 1
    fi
}

echo "Running tests..."
echo "----------------"

for test in "${tests[@]}"; do
    IFS=':' read -r input expected <<<"$test"
    # Handle special case for testing a colon input
    if [[ "$input" == "" && "$expected" == ":Unexpected token ':'" ]]; then
        input=":"
        expected="Unexpected token ':'"
    fi
    run_test "$input" "$expected"
done

# Cleanup
rm -f actual.out actual.err valgrind.log

echo "----------------"
echo "Results:"
echo -e "${GREEN}Passed: $pass_count${NC}"
echo -e "${RED}Failed: $fail_count${NC}"
echo -e "${YELLOW}Tests with memory leaks: $leak_count${NC}"

# Return non-zero exit code if any test failed or had memory leaks
if [ $fail_count -gt 0 ] || [ $leak_count -gt 0 ]; then
    exit 1
else
    exit 0
fi
