#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"

# List of test names
tests=(
  "Valid: Simple number"
  "Valid: Negative number"
  "Valid: Empty object"
  "Valid: Complex object"
  "Valid: Deep nesting"
  "Valid: Multiple escapes"
  "Valid: Big Numbers"

  "Invalid: Unterminated string"
  "Invalid: Invalid escape"
  "Invalid: Missing colon"
  "Invalid: Trailing comma"
  "Invalid: Number after object"
  "Invalid: Nested invalid"
  "Invalid: Double colon"
  "Invalid: Comma before value"
  "Invalid: Non-string key"
  "Invalid: Unterminated object"
  "Invalid: Invalid number"
  "Invalid: Multiple errors"
  "Invalid: Missing closing brace"
  "Invalid: Unexpected token"
  "Invalid: Whitespace error"
)

# Parallel list of inputs
inputs=(
  '123'
  '-456'
  '{}'
  '{"a":42,"b":"value","c":{"nested":-789},"d":"escaped\\\"quote"}'
  '{"a":{"b":{"c":{"d":{"e":42}}}}}'
  '"\\\\\\\""'
  '{"bigNumber":2147483647,"smallNumber":-2147483648}'

  '"hello'
  '"bad\escape"'
  '{"key"42}'
  '{"a":1,}'
  '{}42'
  '{"a":{"b":{"c":}}}'
  '{"key"::42}'
  '{,}'
  '{key:42}'
  '{"a":{'
  '-12a3'
  '{,"a":,"b"}'
  '{"key":"value"'
  ':{\"key\":42}'
  '{"key": 42}'
)

# Parallel list of expected outputs
expected=(
  '123'
  '-456'
  '{}'
  '{"a":42,"b":"value","c":{"nested":-789},"d":"escaped\\\"quote"}'
  '{"a":{"b":{"c":{"d":{"e":42}}}}}'
  '"\\\\\\\""'
  '{"bigNumber":2147483647,"smallNumber":-2147483648}'

  'Unexpected end of input'
  'Unexpected token '\''e'\'''
  'Unexpected token '\''4'\'''
  'Unexpected token '\''}'\'''
  'Unexpected token '\''4'\'''
  'Unexpected token '\''}'\'''
  'Unexpected token '\'':'\'''
  'Unexpected token '\'','\'''
  'Unexpected token '\''k'\'''
  'Unexpected end of input'
  'Unexpected token '\''a'\'''
  'Unexpected token '\'','\'''
  'Unexpected end of input'
  'Unexpected token '\'':'\'''
  'Unexpected token '\'' '\'''
)

total=${#tests[@]}
passed=0

for i in "${!tests[@]}"; do
    name="${tests[i]}"
    input="${inputs[i]}"
    want="${expected[i]}"

    # run the program under test
    echo -n "$input" | ./a.out /dev/stdin >output.txt 2>&1
    status=$?
    got=$(<output.txt)

    if [[ $name == Valid* ]]; then
        if [ $status -eq 0 ] && [ "$got" = "$want" ]; then
            echo -e "${GREEN}PASS:${RESET} $name"
            ((passed++))
        else
            echo -e "${RED}FAIL:${RESET} $name"
            echo -e "  Expected (status 0): $want"
            echo -e "  Got (status $status): $got"
        fi
    else
        if [ $status -eq 1 ] && [[ "$got" == "$want"* ]]; then
            echo -e "${GREEN}PASS:${RESET} $name"
            ((passed++))
        else
            echo -e "${RED}FAIL:${RESET} $name"
            echo -e "  Expected (status 1): $want"
            echo -e "  Got (status $status): $got"
        fi
    fi
done

echo -e "\n${YELLOW}Results: $passed/$total tests passed${RESET}"
rm -f output.txt
exit $((total - passed))
        