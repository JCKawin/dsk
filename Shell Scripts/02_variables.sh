#!/bin/bash
# ==============================================================================
# 02 - Variables, quoting and command substitution
#
# Run: ./02_variables.sh
#
# Concepts: assignment, quoting, ${VAR}, $(command), readonly, read, export
# ==============================================================================

# Assignment: NO spaces around '='
name="OS Lab"
count=5
path="/home/$USER"

echo "name  = $name"
echo "count = $count"
echo "path  = $path"

# Single quotes are literal: no variable expansion inside them.
echo 'Literal: $name is not expanded here'

# Braces separate the variable name from the text that follows it.
echo "File name: ${name}_notes.txt"

# Command substitution: run a command and capture its output.
today=$(date +%F)
kernel=$(uname -s)
echo "Today is $today and the kernel is $kernel"
# The older backtick form also works: `date +%F`

# Arithmetic assignment with $(( ))
total=$((count * 10))
echo "total = $total"

# Read-only variable: changing it is an error.
readonly LAB_NAME="Operating Systems"
echo "LAB_NAME = $LAB_NAME"

# Reading input. If stdin is not a terminal (for example when run_all.sh
# feeds /dev/null) we fall back to a default so the script never hangs.
if [ -t 0 ]; then
    read -r -p "Enter your name: " user_name
else
    user_name="${USER:-student}"
fi
echo "Hello, $user_name"

# Exported variables are copied into child processes.
export GREETING="exported value"
bash -c 'echo "Child process sees GREETING=$GREETING"'

# unset removes a variable; ${var:-default} supplies a value when it is empty.
unset count
echo "count after unset: '${count:-<empty>}'"
