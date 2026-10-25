#!/bin/bash
# ==============================================================================
# 05 - case statement
#
# Run: ./05_case.sh
#
# Concepts: case ... esac, pattern matching, default branch
# ==============================================================================

# Ask for a choice, with a fallback for non-interactive execution.
if [ -t 0 ]; then
    read -r -p "Choose [1-3]: " choice
else
    choice=2
    echo "Choose [1-3]: 2   (non-interactive default)"
fi

case "$choice" in
    1 | start)
        echo "Starting the service..."
        ;;
    2 | stop)
        echo "Stopping the service..."
        ;;
    3 | status)
        echo "Service is running."
        ;;
    *)
        echo "Unknown choice: '$choice'"
        ;;
esac

# case also matches glob patterns, which makes it useful for file extensions.
for file in report.pdf notes.txt photo.jpg README; do
    case "$file" in
        *.pdf)      kind="PDF document" ;;
        *.txt|*.md) kind="text file" ;;
        *.jpg|*.png) kind="image" ;;
        *)          kind="unknown type" ;;
    esac
    printf '%-12s -> %s\n' "$file" "$kind"
done
