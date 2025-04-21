#!/usr/bin/env bash
set -euo pipefail

# Compute n! for n ≥ 0
factorial() {
  local n=$1 result=1 i
  for ((i = 2; i <= n; i++)); do
    result=$((result * i))
  done
  printf "%d" "$result"
}

# List of test‐cases
tests=(
  "a"
  "ab"
  "ba"
  "abc"
  "cba"
  "bac"
  "abcd"
  "badc"
  "dcba"
  "aBc"
  "Aab"
  "123"
  "#\$%"
  "abc@"
  "abcdef"
)

echo "Running $((${#tests[@]})) tests against ./permutations …"
echo

for input in "${tests[@]}"; do
  n=${#input}
  expected=$(factorial "$n")

  # capture output (one permutation per line)
  if ! output=$(./permutations "$input"); then
    echo "❌ Test '$input': program exited non‑zero"
    continue
  fi

  # count and compare
  count=$(printf "%s\n" "$output" | wc -l)
  if [[ $count -ne $expected ]]; then
    echo "❌ Test '$input': expected $expected lines, got $count"
    continue
  fi

  # verify sorted order
  if ! diff -u <(printf "%s\n" "$output") <(printf "%s\n" "$output" | LC_ALL=C sort) >/dev/null; then
    echo "❌ Test '$input': output not in lexicographical order"
    continue
  fi
  # ...existing code...

  echo "✅ Test '$input' (n=$n): $count permutations, sorted"
done
