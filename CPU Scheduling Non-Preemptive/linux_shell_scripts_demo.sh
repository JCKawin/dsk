#!/bin/bash
# ==============================================================================
# Script: linux_shell_scripts_demo.sh
# Purpose: Comprehensive Linux Shell Scripts demonstration covering essential
#          OS concepts:
#          1. Basic file operations & directory management
#          2. Arithmetic operations & even numbers generation
#          3. Conditional statements (if-else)
#          4. Loops (for, while)
#          5. Process monitoring (ps, top, pgrep, pidof)
# ==============================================================================

echo "=========================================================="
echo "         Linux Shell Script Demonstration                 "
echo "=========================================================="

# 1. Print System Information and Current Process Details
echo "[1] Current Process & Shell Details:"
echo "    Script PID       : $$"
echo "    Parent PID (PPID): $PPID"
echo "    Current User     : $(whoami)"
echo "    Current Directory: $(pwd)"
echo ""

# 2. Arithmetic & Even Numbers between 1 to 50
echo "[2] Displaying Even Numbers between 1 to 50 using Shell Loop:"
echo -n "    Even numbers: "
for ((i = 2; i <= 50; i += 2)); do
    echo -n "$i "
done
echo ""
echo ""

# 3. Conditional Check (File existence & Permissions)
echo "[3] Conditional Checks (if-else):"
SAMPLE_FILE="test_file.txt"
if [ -f "$SAMPLE_FILE" ]; then
    echo "    File '$SAMPLE_FILE' already exists."
else
    echo "    Creating '$SAMPLE_FILE'..."
    echo "Operating Systems Shell Scripting Lab" > "$SAMPLE_FILE"
    echo "    File created successfully."
fi

# 4. Display Process Information
echo ""
echo "[4] Process Status Check (ps command):"
echo "    Listing top 5 running processes:"
ps aux --sort=-%cpu 2>/dev/null | head -n 6 || ps -ef | head -n 6

# 5. Cleanup
rm -f "$SAMPLE_FILE"
echo ""
echo "=========================================================="
echo "Shell script execution finished successfully."
echo "=========================================================="
