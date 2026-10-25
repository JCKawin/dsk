#!/bin/bash
# ==============================================================================
# 06 - Loops
#
# Run: ./06_loops.sh
#
# Concepts: for, C-style for, while, until, break, continue, reading a stream
# ==============================================================================

# --- for over a list ---------------------------------------------------------
echo "for over a list:"
for fruit in apple banana cherry; do
    echo "  fruit: $fruit"
done

# --- for over a numeric range ------------------------------------------------
echo "for over a range (1..5):"
for i in {1..5}; do
    echo "  i = $i"
done

# --- C-style for -------------------------------------------------------------
echo "C-style for (even numbers 0..8):"
for ((i = 0; i <= 8; i += 2)); do
    echo -n "  $i"
done
echo

# --- while -------------------------------------------------------------------
echo "while loop (countdown):"
n=3
while [ "$n" -gt 0 ]; do
    echo "  $n..."
    n=$((n - 1))
done
echo "  liftoff!"

# --- until (runs while the condition is FALSE) -------------------------------
echo "until loop:"
m=1
until [ "$m" -gt 3 ]; do
    echo "  m = $m"
    m=$((m + 1))
done

# --- break and continue ------------------------------------------------------
echo "skipping 2 and stopping at 5:"
for i in {1..10}; do
    if [ "$i" -eq 2 ]; then
        continue        # jump to the next iteration
    fi
    if [ "$i" -eq 5 ]; then
        break           # leave the loop entirely
    fi
    echo "  i = $i"
done

# --- reading a stream of lines ----------------------------------------------
echo "reading lines from a here-string:"
while IFS= read -r line; do
    echo "  line: $line"
done <<< "first line
second line
third line"
