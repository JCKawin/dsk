#!/bin/bash
# ==============================================================================
# 09 - String manipulation
#
# Run: ./09_strings.sh
#
# Concepts: length, substring, replacement, prefix/suffix removal,
#           upper/lower case, splitting, default values
# ==============================================================================

text="Shell scripting is fun"
echo "text        : $text"
echo "length      : ${#text}"
echo "first 5     : ${text:0:5}"
echo "from offset 6: ${text:6}"
echo "replace one : ${text/fun/easy}"
echo "replace all : ${text//i/I}"

file="/var/log/syslog.txt"
echo
echo "file        : $file"
echo "directory   : ${file%/*}     (strip shortest '/*' suffix)"
echo "basename    : ${file##*/}    (strip longest '*/' prefix)"
echo "no extension: ${file%.*}"
echo "extension   : ${file##*.}"

word="Hello"
echo
echo "upper       : ${word^^}"
echo "lower       : ${word,,}"

# Trim whitespace with the external xargs (simple and portable).
dirty="   padded text   "
echo "trimmed     : '$(echo "$dirty" | xargs)'"

# Split a string on a delimiter into an array.
csv="red,green,blue"
IFS=',' read -ra colors <<< "$csv"
echo "split on ',' :"
for c in "${colors[@]}"; do
    echo "  color: $c"
done

# Defaults: :- uses a value if the variable is unset or empty.
unset missing
echo "default     : ${missing:-fallback value}"

# Check a prefix / suffix with [[ ]]
if [[ "$file" == *.txt ]]; then
    echo "$file has a .txt suffix"
fi
if [[ "$file" == /var/* ]]; then
    echo "$file lives under /var"
fi
