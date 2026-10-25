#!/bin/bash
# ==============================================================================
# 01 - Hello World: the anatomy of a shell script
#
# Run: ./01_hello_world.sh
#
# Concepts: shebang, comments, echo, quoting, exit status, exit codes
# ==============================================================================

# The first line above is the "shebang". It tells the OS to run the file
# with /bin/bash. Without it the script may be run by /bin/sh instead.

# '#' at the start of a line is a comment. Everything after it is ignored.

echo "Hello, World!"
echo "This script is running as : $0"
echo "Bash version              : ${BASH_VERSION}"
echo "Current user              : ${USER:-unknown}"

# echo has a few useful options:
echo -e "Tabs\tare\thandled\twith\t-e"   # -e interprets \t, \n, etc.
echo -n "This line does not end..."        # -n suppresses the newline
echo " yet."

# Every command returns an exit status: 0 = success, non-zero = failure.
ls / > /dev/null
echo "Status of the last command: $?"

# exit N stops the script and returns N to the caller.
# If you omit it, the script returns the status of its last command.
exit 0
