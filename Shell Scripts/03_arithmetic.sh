#!/bin/bash
# ==============================================================================
# 03 - Arithmetic
#
# Run: ./03_arithmetic.sh
#
# Concepts: $(( )) and (( )), let, expr, integer vs floating point, bc
# ==============================================================================

a=17
b=5
echo "a = $a, b = $b"

# $(( )) does integer arithmetic and returns the result.
echo "a + b  = $((a + b))"
echo "a - b  = $((a - b))"
echo "a * b  = $((a * b))"
echo "a / b  = $((a / b))      <-- integer division truncates"
echo "a % b  = $((a % b))"
echo "a ** 2 = $((a ** 2))     <-- exponent"

# (( )) evaluates an expression without printing it.
((a++))
echo "a after a++ = $a"
((a += 3))
echo "a after a+=3 = $a"

# let is an alternative spelling of (( )).
let "c = a * b"
echo "c = a * b = $c"

# expr is the old external command; quoting and spaces matter.
d=$(expr "$b" + 10)
echo "d = expr $b + 10 = $d"

# Bash has no floating point math. Use bc for decimals (if installed).
if command -v bc > /dev/null 2>&1; then
    echo "a / b = $(echo "scale=3; $a / $b" | bc)   (via bc)"
else
    echo "bc is not installed, skipping decimal division"
fi

# Comparison inside (( )) returns 0 (true) or 1 (false).
if (( a > b )); then
    echo "$a is greater than $b"
fi

# Even/odd check using modulus.
if (( a % 2 == 0 )); then
    echo "$a is even"
else
    echo "$a is odd"
fi
