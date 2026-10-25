#!/bin/bash
# ==============================================================================
# 15 - Common Linux commands
#
# Run: ./15_linux_commands.sh
#
# Concepts: pwd, ls, mkdir, cp, mv, rm, touch, cat, head, tail, grep, cut,
#           sort, uniq, wc, find, xargs, chmod, stat, df, du, uname, whoami,
#           date, ps, command -v, pipes and command substitution
#
# Everything is done inside a temporary directory, so the lab folder is not
# modified.
# ==============================================================================

workdir=$(mktemp -d)
cleanup() {
    rm -rf "$workdir"
    echo "[cleanup] removed $workdir"
}
trap cleanup EXIT

section() {
    printf '\n--- %s ---\n' "$1"
}

section "Where am I? (pwd, ls)"
printf 'pwd: %s\n' "$(pwd)"
echo "ls -la of the lab folder (first 6 entries):"
ls -la | head -n 6

section "Create directories and files (mkdir, touch, printf)"
mkdir -p "$workdir/lab/sub"
printf 'apple 3\nbanana 5\napple 2\ncherry 7\nbanana 1\n' > "$workdir/lab/fruit.txt"
printf 'second file\n' > "$workdir/lab/sub/note.txt"
echo "created:"
find "$workdir" -type f -printf '  %p\n'

section "Copy, move, remove (cp, mv, rm)"
cp "$workdir/lab/fruit.txt" "$workdir/lab/copy.txt"
mv "$workdir/lab/copy.txt" "$workdir/lab/moved.txt"
rm -f "$workdir/lab/moved.txt"
echo "after cp/mv/rm:"
ls -1 "$workdir/lab"

section "View files (cat, head, tail, wc)"
echo "cat fruit.txt:"
cat "$workdir/lab/fruit.txt"
echo "head -n 2:"
head -n 2 "$workdir/lab/fruit.txt"
echo "tail -n 1:"
tail -n 1 "$workdir/lab/fruit.txt"
wc -l "$workdir/lab/fruit.txt"

section "Search and transform (grep, cut, sort, uniq)"
echo "grep apple:"
grep apple "$workdir/lab/fruit.txt"
echo "cut -d' ' -f1:"
cut -d' ' -f1 "$workdir/lab/fruit.txt"
echo "sort | uniq -c:"
cut -d' ' -f1 "$workdir/lab/fruit.txt" | sort | uniq -c | sort -nr

section "Find files and run a command on them (find, xargs)"
find "$workdir" -type f -name '*.txt' -print
find "$workdir" -type f -name '*.txt' -print0 | xargs -0 wc -l

section "Permissions and file metadata (chmod, stat)"
chmod 640 "$workdir/lab/fruit.txt"
stat -c 'name=%n mode=%a size=%s bytes' "$workdir/lab/fruit.txt"

section "System information (uname, whoami, date, df, du)"
printf 'kernel : %s\n' "$(uname -s)"
printf 'release: %s\n' "$(uname -r)"
printf 'user   : %s\n' "$(whoami)"
printf 'date   : %s\n' "$(date '+%Y-%m-%d %H:%M:%S %Z')"
df -h . | head -n 2
du -sh "$workdir" 2>/dev/null

section "Processes and command lookup (ps, command -v)"
ps -p $$ -o pid=,ppid=,stat=,comm= 2>/dev/null || echo "ps not available"
for cmd in bash gcc find grep; do
    if command -v "$cmd" > /dev/null 2>&1; then
        printf '%-6s %s\n' "$cmd" "$(command -v "$cmd")"
    else
        printf '%-6s %s\n' "$cmd" "not installed"
    fi
done

section "Quoting and pipes"
count=$(wc -l < "$workdir/lab/fruit.txt")
echo "the file has $count lines"
echo "one two three" | tr ' ' '\n' | sort -r

echo
echo "Linux commands lesson complete."
