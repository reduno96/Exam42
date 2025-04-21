#!/usr/bin/env bash
set -uo pipefail # Removed -e to prevent exiting on first error

# --- CONFIGURE THIS ---
SRC="powerset.c" # your source file
EXE="powerset"   # compiled binary name

# Compile
gcc -Wall -Wextra -Werror "$SRC" -o "$EXE" || {
  echo "✖ Compilation failed"
  exit 1
}

# Define tests in the form: "ARGS|EXPECTED_OUTPUT"
# (For no-output tests, leave EXPECTED empty)
# Expected outputs are based on the problem description and C code logic.
tests=(
  "1 1|1"       # Target 1, Set {1} -> {1}
  "5 3|"        # Target 5, Set {3} -> (none)
  "0 0|0"       # Target 0, Set {0} -> {0}
  "0 1 -1|1 -1" # Target 0, Set {1, -1} -> {1, -1}
  "5 5 2 3|2 3
5"                     # Target 5, Set {5, 2, 3} -> {2, 3}, {5}
  "10 1 2 3 4|1 2 3 4" # Target 10, Set {1, 2, 3, 4} -> {1, 2, 3, 4}
  "2 1 -1 2|1 -1 2
2"                # Target 2, Set {1, -1, 2} -> {1, -1, 2}, {2}
  "7 3 8 2|"      # Target 7, Set {3, 8, 2} -> (none)
  "7 3 8 2 4|3 4" # Target 7, Set {3, 8, 2, 4} -> {3, 4}
  "3 1 0 2 4 5 3|1 0 2
1 2
0 3
3" # Target 3, Set {1, 0, 2, 4, 5, 3} -> {1, 0, 2}, {1, 2}, {0, 3}, {3}
  "12 5 2 1 8 4 3 7 11|5 2 1 4
5 4 3
5 7
2 3 7
1 8 3
1 4 7
1 11
8 4" # Target 12, Set {5, 2, 1, 8, 4, 3, 7, 11} -> {5, 2, 1, 4}, {5, 4, 3}, {5, 7}, {2, 3, 7}, {1, 8, 3}, {1, 4, 7}, {1, 11}, {8, 4}
  "100 10 20 30 40 50|10 20 30 40
20 30 50
10 40 50" # Target 100, Set {10, 20, 30, 40, 50} -> {10, 20, 30, 40}, {20, 30, 50}, {10, 40, 50}
  "1 1 0 -1 2|0 -1 2
-1 2
1 0
1" # Target 1, Set {1, 0, -1, 2} -> {0, -1, 2}, {-1, 2}, {1, 0}, {1}
  "-3 -1 -2 -3 1|-1 -3 1
-1 -2
-3" # Target -3, Set {-1, -2, -3, 1} -> {-1, -3, 1}, {-1, -2}, {-3}
  "6 1 2 3 4 5|2 4
1 5
1 2 3" # Target 6, Set {1, 2, 3, 4, 5} -> {2, 4}, {1, 5}, {1, 2, 3}
)
pass=0
fail=0
echo "Running ${#tests[@]} tests…"
for t in "${tests[@]}"; do
  # split on the first '|' — this preserves any newlines in $expected
  args="${t%%|*}"
  expected="${t#*|}"

  # run and trim trailing whitespace
  actual=$("./$EXE" $args 2>&1 || true | sed 's/[[:space:]]*$//')

  # sort both sets of lines for order‑independent compare
  sorted_act=$(printf '%s\n' "$actual" | sort)
  sorted_exp=$(printf '%s\n' "$expected" | sort)

  if [[ "$sorted_act" == "$sorted_exp" ]]; then
    echo "✔ [PASS] args=($args)"
    ((pass++))
  else
    echo "✖ [FAIL] args=($args)"
    echo "    Expected (sorted):"
    echo "---"
    printf '%s\n' "$sorted_exp" | sed 's/^/      /'
    echo "---"
    echo "    Got (sorted):"
    echo "---"
    printf '%s\n' "$sorted_act" | sed 's/^/      /'
    echo "---"
    ((fail++))
  fi
done

echo
echo "Summary: $pass passed, $fail failed."
# Exit with the number of failures (0 if all passed)
exit $fail
