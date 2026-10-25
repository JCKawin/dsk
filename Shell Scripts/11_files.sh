#!/bin/bash
# ==============================================================================
# 11 - File operations and redirection
#
# Run: ./11_files.sh
#
# Concepts: >, >>, 2>, tee, mktemp, trap, read loop, wc, find
# ==============================================================================

# Work in a temporary directory so the lab folder stays clean.
workdir=$(mktemp -d)
cleanup() {
    rm -rf "$workdir"
    echo "[cleanup] removed $workdir"
}
trap cleanup EXIT          # runs automatically when the script exits

file="$workdir/notes.txt"

# '>' overwrites, '>>' appends.
echo "Operating Systems Lab" > "$file"
echo "Shell scripting basics" >> "$file"
printf 'Line %d\n' 3 4 >> "$file"

echo "--- contents of $file ---"
cat "$file"

# Count lines, words and characters.
echo "--- wc ---"
wc -l "$file"

# Read the file line by line. IFS= and -r keep spaces and backslashes intact.
echo "--- numbered lines ---"
n=0
while IFS= read -r line; do
    n=$((n + 1))
    echo "  $n: $line"
done < "$file"

# Redirect errors to /dev/null and handle the failure ourselves.
if ls "$workdir/does_not_exist" 2> /dev/null; then
    echo "unexpected"
else
    echo "missing file handled cleanly (stderr was discarded)"
fi

# tee writes to both the screen and a file.
echo "--- tee ---"
echo "both screen and file" | tee "$workdir/tee.txt"
echo "tee.txt contains: $(cat "$workdir/tee.txt")"

# File tests and a small find.
if [ -s "$file" ]; then
    echo "$file is non-empty"
fi
echo "--- files in workdir ---"
find "$workdir" -type f -printf '  %f\n' 2>/dev/null || ls "$workdir"

# The trap removes the directory even if an earlier command fails.
