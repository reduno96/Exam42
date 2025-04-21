#!/bin/bash

# Compile with AddressSanitizer
gcc -fsanitize=address -g -Wall -Wextra -Werror -o argo argo.c main.c || exit 1

# Test cases
tests=(
    "1|1"
    "\"bonjour\"|\"bonjour\""
    "\"escape!\\\"\"|\"escape!\\\"\""
    "{\"tomatoes\":42,\"potatoes\":234}|{\"tomatoes\":42,\"potatoes\":234}"
    "{\"recursion\":{\"recursion\":{\"recursion\":\"recursion\"}}}|{\"recursion\":{\"recursion\":{\"recursion\":\"recursion\"}}}"
    "\"unfinished string|Unexpected end of input"
    "\"unfinished string 2\\\"|Unexpected end of input"
    "{\"no value?\":}|Unexpected token '}'"
    "{:12}|Unexpected token ':'"
)

# Run tests and check for leaks
for test in "${tests[@]}"; do
    echo "Testing input: $test"
    echo -n "$test" >test_input.json
    # Run the program and check for leaks
    output=$(./argo test_input.json 2>&1)
    # Check if ASAN reported errors
    if [[ "$output" == *"LeakSanitizer"* ]]; then
        echo "MEMORY LEAK DETECTED in test: $test"
        echo "$output"
        exit 1
    else
        echo "Test PASSED (no leaks): $test"
    fi
    rm -f test_input.json
    echo "--------------------"
done

# Cleanup
rm -f argo
