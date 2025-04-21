#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"

declare -A tests=(
    ["Valid: Simple number"]='123'
    ["Valid: Negative number"]='-456'
    ["Valid: Empty object"]='{}'
    ["Valid: Complex object"]='{"a":42,"b":"value","c":{"nested":-789},"d":"escaped\\\"quote"}'
    ["Valid: Deep nesting"]='{"a":{"b":{"c":{"d":{"e":42}}}}}'
    ["Valid: Multiple escapes"]='"\\\\\\\""'
    ["Valid: Big Numbers"]='{"bigNumber":2147483647,"smallNumber":-2147483648}'

    ["Invalid: Unterminated string"]='"hello'
    ["Invalid: Invalid escape"]='"bad\escape"'
    ["Invalid: Missing colon"]='{"key"42}'
    ["Invalid: Trailing comma"]='{"a":1,}'
    ["Invalid: Number after object"]='{}42'
    ["Invalid: Nested invalid"]='{"a":{"b":{"c":}}}'
    ["Invalid: Double colon"]='{"key"::42}'
    ["Invalid: Comma before value"]='{,}'
    ["Invalid: Non-string key"]='{key:42}'
    ["Invalid: Unterminated object"]='{"a":{'
    ["Invalid: Invalid number"]='-12a3'
    ["Invalid: Multiple errors"]='{,"a":,"b"}'
    ["Invalid: Missing closing brace"]='{"key":"value"'
    ["Invalid: Unexpected token"]=':{"key":42}'
    ["Invalid: Whitespace error"]='{"key": 42}'
)

declare -A expectations=(
    ["Valid: Simple number"]='123'
    ["Valid: Negative number"]='-456'
    ["Valid: Empty object"]='{}'
    ["Valid: Complex object"]='{"a":42,"b":"value","c":{"nested":-789},"d":"escaped\\\"quote"}'
    ["Valid: Deep nesting"]='{"a":{"b":{"c":{"d":{"e":42}}}}}'
    ["Valid: Multiple escapes"]='"\\\\\\\""'
    ["Valid: Big Numbers"]='{"bigNumber":2147483647,"smallNumber":-2147483648}'

    ["Invalid: Unterminated string"]='Unexpected end of input'
    ["Invalid: Invalid escape"]='Unexpected token '\''e'\'''
    ["Invalid: Missing colon"]='Unexpected token '\''4'\'''
    ["Invalid: Trailing comma"]='Unexpected token '\''}'\'''
    ["Invalid: Number after object"]='Unexpected token '\''4'\'''
    ["Invalid: Nested invalid"]='Unexpected token '\''}'\'''
    ["Invalid: Double colon"]='Unexpected token '\'':'\'''
    ["Invalid: Comma before value"]='Unexpected token '\'','\'''
    ["Invalid: Non-string key"]='Unexpected token '\''k'\'''
    ["Invalid: Unterminated object"]='Unexpected end of input'
    ["Invalid: Invalid number"]='Unexpected token '\''a'\'''
    ["Invalid: Multiple errors"]='Unexpected token '\'','\'''
    ["Invalid: Missing closing brace"]='Unexpected end of input'
    ["Invalid: Unexpected token"]='Unexpected token '\'':'\'''
    ["Invalid: Whitespace error"]='Unexpected token '\'' '\'''
)

total=0
passed=0

for testname in "${!tests[@]}"; do
    ((total++))
    input="${tests[$testname]}"
    expected="${expectations[$testname]}"

    echo -n "$input" | ./a.out /dev/stdin >output.txt 2>&1
    result=$?
    output=$(cat output.txt)

    if [[ $testname == Valid* ]]; then
        # Valid tests should return 0 exit code
        if [ $result -eq 0 ] && [ "$output" = "$expected" ]; then
            echo -e "${GREEN}PASS: $testname${RESET}"
            ((passed++))
        else
            echo -e "${RED}FAIL: $testname"
            echo -e "  Expected (status 0): $expected"
            echo -e "  Got (status $result): $output${RESET}"
        fi
    else
        # Invalid tests should return 1 exit code
        if [ $result -eq 1 ] && [[ "$output" == "$expected"* ]]; then
            echo -e "${GREEN}PASS: $testname${RESET}"
            ((passed++))
        else
            echo -e "${RED}FAIL: $testname"
            echo -e "  Expected (status 1): $expected"
            echo -e "  Got (status $result): $output${RESET}"
        fi
    fi
done

echo -e "\n${YELLOW}Results: $passed/$total tests passed${RESET}"
rm output.txt
exit $((total - passed))
