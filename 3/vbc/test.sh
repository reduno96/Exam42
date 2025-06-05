#!/bin/bash

# Compile the vbc.c source file into an executable named "vbc"
gcc -Wall -Wextra -fsanitize=address -o vbc vbc.c main.c
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful."

# Function that runs a single test case and compares the output and exit code.
# Arguments:
#   $1 - the input expression to test
#   $2 - the expected output string (error messages or result)
#   $3 - the expected exit code (0 for success, 1 for error)
run_test() {
    input="$1"
    expected="$2"
    expected_exit_code="$3"

    # Run the executable with the provided input and capture its output and exit status.
    output=$(./vbc "$input" 2>&1)
    exit_code=$?

    # Remove any trailing newlines for accurate comparison.
    output=$(echo "$output" | tr -d '\n')

    if [ "$output" = "$expected" ] && [ $exit_code -eq $expected_exit_code ]; then
        echo "Test with input '$input' PASSED"
    else
        echo "Test with input '$input' FAILED"
        echo "  Expected: '$expected' (exit $expected_exit_code)"
        echo "  Got:      '$output' (exit $exit_code)"
    fi
}

# --- Valid Expressions ---
# Each valid test should return the computed value and have an exit code of 0.
run_test "(3+2)*4" "20" 0
run_test "3+2*4" "11" 0
run_test "((2+1)*3)+4*(2+(3+2))" "37" 0

# --- Error Handling Tests ---
# Test cases for error conditions.
# The program is expected to print an error message and exit with code 1.

# Unexpected token: In the expression "3a+2", the character 'a' is not allowed.
run_test "3a+2" "Unexpected token 'a'" 1

# Unexpected end of input: In the expression "(3+2", a closing parenthesis is missing.
run_test "(3+2" "Unexpected end of input" 1
