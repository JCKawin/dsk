#!/bin/bash
# ==============================================================================
# 13 - System information report
#
# Run: ./13_system_info.sh
#
# Concepts: combining commands, command substitution, formatting with printf,
#           checking that a command exists before using it
# ==============================================================================

section() {
    printf '\n--- %s ---\n' "$1"
}

section "Identity"
printf '%-14s %s\n' "Hostname:" "$(uname -n)"
printf '%-14s %s\n' "User:"     "$(whoami)"
printf '%-14s %s\n' "Shell:"    "${SHELL:-$0}"
printf '%-14s %s\n' "Uptime:"   "$(uptime -p 2>/dev/null || uptime)"

section "Kernel / OS"
printf '%-14s %s\n' "Kernel:"   "$(uname -s)"
printf '%-14s %s\n' "Release:"  "$(uname -r)"
printf '%-14s %s\n' "Machine:"  "$(uname -m)"
printf '%-14s %s\n' "Date:"     "$(date '+%Y-%m-%d %H:%M:%S %Z')"

section "CPU"
if command -v nproc > /dev/null 2>&1; then
    printf '%-14s %s\n' "CPU cores:" "$(nproc)"
elif [ -r /proc/cpuinfo ]; then
    printf '%-14s %s\n' "CPU cores:" "$(grep -c '^processor' /proc/cpuinfo)"
fi
if [ -r /proc/loadavg ]; then
    printf '%-14s %s\n' "Load average:" "$(cut -d' ' -f1-3 /proc/loadavg)"
fi

section "Memory"
if command -v free > /dev/null 2>&1; then
    free -h
elif [ -r /proc/meminfo ]; then
    head -n 3 /proc/meminfo
else
    echo "memory information not available"
fi

section "Disk usage of the current directory"
df -h . 2>/dev/null || echo "df not available"

section "Largest items in this folder"
du -sh ./* 2>/dev/null | sort -h | tail -n 5 || echo "du not available"

section "Logged-in users"
who 2>/dev/null || echo "no interactive users"
