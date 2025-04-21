#!/usr/bin/env bash

PROG=./a.out
OK=0
FAIL=0

run_test() {
  local input="$1"
  shift
  local expected=("$@")
  # Capture program output, one line per array element (cat -e appends '$' to end of each line)
  local got
  IFS=$'\n' read -d '' -r -a got < <($PROG "$input" | cat -e) || true

  # Sort both expected and got so order doesn’t matter
  IFS=$'\n' sorted_expected=($(printf "%s\n" "${expected[@]}" | sort))
  IFS=$'\n' sorted_got=($(printf "%s\n" "${got[@]}" | sort))

  if [ "${#sorted_expected[@]}" -eq "${#sorted_got[@]}" ] && [ "${sorted_expected[*]}" = "${sorted_got[*]}" ]; then
    echo "✅ PASS: '$input'"
    ((OK++))
  else
    echo "❌ FAIL: '$input'"
    echo "   Expected (${#expected[@]}):"
    printf "     %s\n" "${expected[@]}"
    echo "   Got      (${#got[@]}):"
    printf "     %s\n" "${got[@]}"
    ((FAIL++))
  fi
}

# 15 Test Cases

run_test "" # 1: empty input, no output

run_test "()" "()$"     # 2: already balanced simple
run_test "(())" "(())$" # 3: already balanced nested

run_test "(()" " ()$" "( )$" # 4: single extra open
run_test "())" "() $" "( )$" # 5: single extra close

run_test "))))" "    $"   # 6: only closes
run_test "(((((" "     $" # 7: only opens

run_test ")(" "  $" # 8: unbalanced both ends

run_test "((()()())())" "((()()())())$" # 9: complex balanced (no removals)

run_test "()())()" "()() ()$" "()( )()$" "( ())()$" # 10: example from prompt

run_test "(()(()(" "(()  ) $" "( )( ) $" "( ) () $" " ()( ) $" " () () $"
# 11: example from prompt

run_test ")()(" " () $" # 12: unbalanced in the middle

run_test "()(()" "() ()$" "()( )$" # 13: one extra open in middle

# ...existing code...

# Test 14: Input '(())())' (remove 1 ')')
# Original expected: "(())( )$" "(())() $"
# Actual output includes: "(( )())$", "(() ())$", "(())( )$", "(())() $"
run_test "(())())" "(( )())$" "(() ())$" "(())( )$" "(())() $" # 14: one extra close in nested

# Test 15: Input '(()))(' (remove 1 '(', 1 ')')
# Original expected: "(())  $"
# Actual output includes: "(( )) $", "(() ) $", "(())  $"
run_test "(()))(" "(( )) $" "(() ) $" "(())  $" # 15: one extra close + one extra open

# ...existing code...
echo
echo "Summary: $OK passed, $FAIL failed"
exit $FAIL
