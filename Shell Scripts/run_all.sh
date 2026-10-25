#!/bin/bash
# ==============================================================================
# run_all.sh - run every lesson script in order
#
# Run: ./run_all.sh
#
# Each script runs in a child bash process with stdin from /dev/null, so the
# interactive prompts fall back to their non-interactive defaults.
# ==============================================================================

# Work from the folder that contains this script, no matter where it is called.
cd "$(dirname "$0")" || exit 1

scripts=(
    01_hello_world.sh
    02_variables.sh
    03_arithmetic.sh
    04_conditions.sh
    05_case.sh
    06_loops.sh
    07_functions.sh
    08_arrays.sh
    09_strings.sh
    10_args.sh
    11_files.sh
    12_processes.sh
    13_system_info.sh
    14_menu.sh
    15_linux_commands.sh
)

failed=0
for script in "${scripts[@]}"; do
    printf '\n============================================================\n'
    printf '>>> %s\n' "$script"
    printf '============================================================\n'
    if ! bash "$script" < /dev/null; then
        printf '!!! %s failed\n' "$script"
        failed=$((failed + 1))
    fi
done

printf '\n============================================================\n'
if [ "$failed" -eq 0 ]; then
    echo "All ${#scripts[@]} scripts finished successfully."
else
    echo "$failed of ${#scripts[@]} scripts failed."
fi
printf '============================================================\n'

exit "$failed"
