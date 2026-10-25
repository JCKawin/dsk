#!/bin/bash
# ==============================================================================
# 10 - Command line arguments and options
#
# Run: ./10_args.sh hello world
#      ./10_args.sh -n Alice -v
#
# Concepts: $0 $1 $# $@ $*, shift, getopts, usage messages
# ==============================================================================

echo "script name : $0"
echo "argument count : $#"
echo "all arguments (\"\$@\") :"
for arg in "$@"; do
    echo "  - $arg"
done

# $* joins all arguments into one word; "$@" keeps them separate.
echo "joined with \$* : $*"

# If no arguments were given, supply demo values so the script still runs.
if [ "$#" -eq 0 ]; then
    echo "(no arguments supplied, using demo values)"
    set -- one two three
fi

# shift drops $1 and moves the rest down.
echo "first argument : $1"
shift
echo "after shift    : $*"

# --- Options with getopts ----------------------------------------------------
# -n takes a value, -v is a flag. getopts reads the original arguments, so
# we reset them for this demo.
set -- -n Alice -v
name=""
verbose=0
while getopts ":n:v" opt; do
    case "$opt" in
        n) name="$OPTARG" ;;
        v) verbose=1 ;;
        :) echo "Option -$OPTARG needs a value" >&2 ;;
        \?) echo "Unknown option -$OPTARG" >&2 ;;
    esac
done
shift $((OPTIND - 1))

echo "name option     : ${name:-<not given>}"
echo "verbose flag    : $verbose"
echo "remaining args  : $*"
