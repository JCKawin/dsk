#!/bin/bash
# ==============================================================================
# 12 - Processes, jobs and signals
#
# Run: ./12_processes.sh
#
# Concepts: $$, $!, ps, background jobs, wait, kill, exit status, pgrep
# ==============================================================================

# ps prints a "screen size is bogus" warning when COLUMNS/LINES are unusable
# (common when there is no terminal). Set them to keep the output clean.
export COLUMNS=120 LINES=24

echo "this script PID   : $$"
echo "parent shell PID  : $PPID"

# Run a command in the background and remember its PID with $!.
sleep 2 &
child_pid=$!
echo "started 'sleep 2' with PID $child_pid"

# ps shows the process while it is still running.
if ps -p "$child_pid" -o pid=,stat=,comm= > /dev/null 2>&1; then
    ps -p "$child_pid" -o pid=,stat=,comm=
fi

# wait blocks until the child finishes and returns its exit status.
wait "$child_pid"
echo "child finished with status $?"

# Start a long job, then terminate it on purpose.
sleep 30 &
victim=$!
echo "started 'sleep 30' with PID $victim"
kill "$victim" 2> /dev/null
wait "$victim" 2> /dev/null
echo "terminated PID $victim (status $?)"

# kill -0 only checks whether a process exists; it does not send a signal.
if kill -0 "$$" 2> /dev/null; then
    echo "this script is still alive"
fi

# A quick look at the busiest processes (fallbacks for minimal systems).
echo "--- top CPU consumers ---"
ps -eo pid,comm,%cpu --sort=-%cpu 2> /dev/null | head -n 6 \
    || ps -ef 2> /dev/null | head -n 6 \
    || echo "ps not available"

# pgrep searches by name.
if command -v pgrep > /dev/null 2>&1; then
    echo "bash processes: $(pgrep -c bash 2> /dev/null || echo 0)"
fi
