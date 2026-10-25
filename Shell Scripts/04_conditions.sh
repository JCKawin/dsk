#!/bin/bash
# ==============================================================================
# 04 - Conditional statements
#
# Run: ./04_conditions.sh
#
# Concepts: if / elif / else, [ ] vs [[ ]], numeric and string tests,
#           file tests, combining conditions
# ==============================================================================

num=42
name="shell"

# --- Numeric comparisons: -eq -ne -lt -le -gt -ge ------------------------------
if [ "$num" -eq 42 ]; then
    echo "$num is equal to 42"
fi

if [ "$num" -gt 100 ]; then
    echo "$num is greater than 100"
elif [ "$num" -gt 10 ]; then
    echo "$num is greater than 10 (but not greater than 100)"
else
    echo "$num is 10 or less"
fi

# --- String comparisons: = != -z (empty) -n (not empty) -----------------------
if [ "$name" = "shell" ]; then
    echo "name equals 'shell'"
fi

empty=""
if [ -z "$empty" ]; then
    echo "variable 'empty' has zero length"
fi

# [[ ]] is a bash extension: safer word splitting and pattern matching.
if [[ "$name" == sh* ]]; then
    echo "name starts with 'sh' (pattern match with [[ ]])"
fi

# [[ ]] can also combine tests with && and ||.
if [[ "$name" == "shell" && "$num" -gt 0 ]]; then
    echo "both conditions are true"
fi

# --- File tests: -e -f -d -r -w -x -s ----------------------------------------
tmp_file=$(mktemp)
echo "temporary file: $tmp_file"

if [ -e "$tmp_file" ]; then echo "the file exists          (-e)"; fi
if [ -f "$tmp_file" ]; then echo "it is a regular file     (-f)"; fi
if [ -d "/tmp" ];     then echo "/tmp is a directory      (-d)"; fi
if [ -s "$tmp_file" ]; then echo "file is non-empty        (-s)"; fi
if [ -w "$tmp_file" ]; then echo "file is writable         (-w)"; fi
rm -f "$tmp_file"

# --- A command's exit status can be the condition -----------------------------
if command -v bash > /dev/null 2>&1; then
    echo "bash is available"
fi

# --- Old-style [ ] tests need explicit && between them -----------------------
if [ "$num" -gt 0 ] && [ "$num" -lt 100 ]; then
    echo "$num is between 1 and 99"
fi
