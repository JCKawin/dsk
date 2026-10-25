#!/bin/bash
# ==============================================================================
# 08 - Arrays
#
# Run: ./08_arrays.sh
#
# Concepts: indexed arrays, ${arr[@]}, ${#arr[@]}, slices, associative arrays
# ==============================================================================

# Indexed array. Indices start at 0.
fruits=("apple" "banana" "cherry")

echo "first element  : ${fruits[0]}"
echo "all elements   : ${fruits[@]}"
echo "element count  : ${#fruits[@]}"
echo "all indices    : ${!fruits[@]}"

# Append and assign by index.
fruits+=("date")
fruits[5]="elderberry"        # index 4 stays empty
echo "after adding   : ${fruits[@]}"

# Loop over values.
echo "values:"
for fruit in "${fruits[@]}"; do
    echo "  - $fruit"
done

# Loop over indices (useful when you need the position).
echo "indices:"
for i in "${!fruits[@]}"; do
    echo "  [$i] ${fruits[$i]}"
done

# Slice: ${array[@]:start:count}
echo "slice [1..2]   : ${fruits[@]:1:2}"

# Remove an element (leaves a gap at index 1).
unset 'fruits[1]'
echo "after unset[1] : ${fruits[@]}"

# --- Associative (key/value) arrays, bash 4+ --------------------------------
declare -A marks
marks[math]=90
marks[os]=85
marks[shell]=95

echo "associative array:"
for key in "${!marks[@]}"; do
    echo "  $key = ${marks[$key]}"
done

# Sort the keys for stable output.
echo "sorted keys:"
for key in $(printf '%s\n' "${!marks[@]}" | sort); do
    echo "  $key -> ${marks[$key]}"
done
