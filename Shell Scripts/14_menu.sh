#!/bin/bash
# ==============================================================================
# 14 - Interactive menu (puts several concepts together)
#
# Run: ./14_menu.sh
#
# Concepts: functions, while loop, case, read, here-document, input validation
# ==============================================================================

show_menu() {
    cat <<'EOF'
+-----------------------------+
|        OS Lab Menu          |
+-----------------------------+
| 1) Show date and time       |
| 2) Show current directory   |
| 3) Show disk usage          |
| 4) Count files in a folder  |
| 5) Quit                     |
+-----------------------------+
EOF
}

do_date() {
    echo "Now: $(date '+%A, %d %B %Y at %H:%M:%S')"
}

do_dir() {
    echo "Working directory: $(pwd)"
    ls -1 | head -n 10
}

do_disk() {
    df -h . 2>/dev/null || echo "df is not available"
}

count_files() {
    local folder="$1"
    if [ ! -d "$folder" ]; then
        echo "'$folder' is not a directory" >&2
        return 1
    fi
    # -maxdepth 1 keeps the count to this folder only.
    find "$folder" -maxdepth 1 -type f | wc -l
}

run_choice() {
    case "$1" in
        1) do_date ;;
        2) do_dir ;;
        3) do_disk ;;
        4)
            local target
            read -r -p "Folder [.]: " target
            target=${target:-.}
            echo "Files in '$target': $(count_files "$target")"
            ;;
        5|q|quit) echo "Goodbye."; return 1 ;;
        *) echo "Invalid choice: '$1'" ;;
    esac
    return 0
}

# In a real terminal, loop until the user quits.
if [ -t 0 ]; then
    while true; do
        show_menu
        read -r -p "Choice [1-5]: " choice || break
        run_choice "$choice" || break
        echo
    done
else
    # Non-interactive run (for example from run_all.sh): show one option.
    echo "(non-interactive mode) running option 1"
    run_choice 1
fi
