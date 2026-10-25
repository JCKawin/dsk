#!/bin/bash
# ==============================================================================
# 07 - Functions
#
# Run: ./07_functions.sh
#
# Concepts: definition, arguments, printing vs returning, local, recursion
# ==============================================================================

# Arguments arrive as $1, $2, ... and the count is $#.
greet() {
    echo "Hello, $1! You are visitor number $2."
}

# A function "returns" data by printing it; the caller captures it with $( ).
add() {
    local sum=$(( $1 + $2 ))
    echo "$sum"
}

# A function can also use the exit status: 0 means true/success.
is_even() {
    if (( $1 % 2 == 0 )); then
        return 0
    else
        return 1
    fi
}

# Recursion works, but use 'local' for the variables.
factorial() {
    local n=$1
    if (( n <= 1 )); then
        echo 1
        return
    fi
    local sub
    sub=$(factorial $((n - 1)))
    echo $((n * sub))
}

greet "OS Lab" 1

result=$(add 12 30)
echo "12 + 30 = $result"

if is_even 42; then
    echo "42 is even"
else
    echo "42 is odd"
fi

echo "5! = $(factorial 5)"

# --- local vs global variables ----------------------------------------------
counter=0
bump() {
    local counter=99          # shadows the global variable
    counter=$((counter + 1))
    echo "inside  function: counter=$counter"
}
bump
echo "outside function: counter=$counter"

# Functions must be defined before they are called.
# List everything that is currently defined with: declare -f
