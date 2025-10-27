#!/bin/bash

# Test results for LLVM IR generation

echo "LLVM IR Generation Test Results"
echo "================================"
echo ""

tests=(
  "simple.ct:8:5 + 3"
  "nested.ct:19:(2+3)*4-1"
  "iftest.ct:5:if true then 5 else 3"
  "logic.ct:1:5 > 3 && 2 < 4"
  "nested_if.ct:2:if true then (if false then 1 else 2) else 3"
)

for test in "${tests[@]}"; do
  IFS=':' read -r file expected desc <<< "$test"
  if [ -f "$file" ]; then
    actual=$(grep "ret i32" "${file%.ct}.ll" | grep -o "[0-9]*$" | tail -1)
    status="✓"
    if [ "$actual" != "$expected" ]; then
      status="✗"
    fi
    printf "%-25s Expected: %3d  Got: %3d  %s\n" "$desc" "$expected" "$actual" "$status"
  fi
done

echo ""
echo "All IR files generated in current directory:"
ls -lh *.ll 2>/dev/null | awk '{print $9, "(" $5 ")"}'
