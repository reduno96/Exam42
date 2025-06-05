#!/usr/bin/env bash

# Compile
clear
cc -Wall -Wextra -Werror main.c argo.c -o argo

separator="------------------------------"

run_test() {
    local input="$1"
    local desc="$2"
    echo "$separator"
    echo "TEST CASE: $desc"
    printf '%s' "$input" > input.txt
    echo -n "ARGO PRINTED: "
    ./argo input.txt
    local status=$?
    echo -n "RETURNED: "
    if [ $status -eq 0 ]; then
        echo "correct"
    else
        echo "error (status $status)"
    fi
    echo "$separator"
}

run_test_nl() {
    local input="$1"
    local desc="$2"
    echo "$separator"
    echo "TEST CASE: $desc  (with newline)"
    printf '%s\n' "$input" > input.txt
    echo -n "ARGO PRINTED: "
    ./argo input.txt
    local status=$?
    echo -n "RETURNED: "
    if [ $status -eq 0 ]; then
        echo "correct"
    else
        echo "error (status $status)"
    fi
    echo "$separator"
}

# --- Tests without trailing newline ---
run_test '1'                     '1'
run_test '"bonjour"'             '"bonjour"'
run_test '"escape! \" "'        '"escape! \" "'
run_test '{"tomatoes":42,"potatoes":234}'  'simple object'
run_test '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}'  'deep object'
run_test '"unfinished string'    'unterminated string'
run_test '"unfinished string 2\"' 'unterminated+escaped quote'
run_test '{"no value?":}'        'missing value'

# --- Tests that must include a final newline in the input ---
for num in 1 0 -1 213213 2313213 -- --1; do
    run_test_nl "$num" "$num"
done

run_test '""'                    'empty string'
run_test '"HELLO WORLD"'        '"HELLO WORLD"'
run_test '"HELLO WORLD'         'unterminated hello'
run_test '{:42}'                '{:42}'
run_test '{a:42}'               '{a:42}'
run_test '{"1":"","1":"1","1":"2"}'   'duplicate keys'
run_test '{"":"","":"","":""}'         'all-empty keys'
run_test '{"1":"1","2":"2","3":{"11":11,"22":22,"33":{"111":111}}}' 'mixed nested'

# Cleanup
rm -f input.txt
