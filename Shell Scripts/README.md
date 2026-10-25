# Shell Script Basics (Bash)

A small, self-contained set of Linux shell scripting examples for the OS Lab.
Each file is a runnable lesson focused on one topic, and every script prints
what it is doing so you can read the output as notes.

## Files

| File | Topic |
| ---- | ----- |
| `01_hello_world.sh` | shebang, comments, `echo`, exit status |
| `02_variables.sh` | variables, quoting, `$( )`, `read`, `export`, `readonly` |
| `03_arithmetic.sh` | `$(( ))`, `(( ))`, `let`, `expr`, integer vs `bc` |
| `04_conditions.sh` | `if/elif/else`, `[ ]` vs `[[ ]]`, numeric/string/file tests |
| `05_case.sh` | `case ... esac`, pattern matching |
| `06_loops.sh` | `for`, C-style `for`, `while`, `until`, `break`/`continue` |
| `07_functions.sh` | functions, arguments, return values, `local`, recursion |
| `08_arrays.sh` | indexed arrays, slices, associative arrays |
| `09_strings.sh` | substring, replacement, prefix/suffix, case, splitting |
| `10_args.sh` | `$0 $1 $# $@ $*`, `shift`, `getopts` |
| `11_files.sh` | redirection, `tee`, `mktemp`, `trap`, `read` loop, `find` |
| `12_processes.sh` | `$$`, `$!`, background jobs, `wait`, `kill`, `ps` |
| `13_system_info.sh` | system report using `uname`, `free`, `df`, `du` |
| `14_menu.sh` | interactive menu combining functions, `case` and loops |
| `15_linux_commands.sh` | common commands: `ls`, `cp`, `grep`, `sort`, `find`, `chmod`, `df`, `ps` |
| `run_all.sh` | runs lessons 01-15 in order |

## How to run (Linux / WSL / Git Bash)

```bash
cd "Shell Scripts"
chmod +x *.sh          # only needed once
./01_hello_world.sh    # run one lesson
./run_all.sh           # run everything, in order
```

You can also run a script without the executable bit:

```bash
bash 03_arithmetic.sh
```

All scripts are written for `bash` (not plain `sh`). `08_arrays.sh` needs
bash 4+ for associative arrays, which is the default on current Linux systems.

## Running with arguments

`10_args.sh` demonstrates positional parameters and options:

```bash
./10_args.sh hello world
./10_args.sh -n Alice -v
```

## Notes for Windows users

- Run these from WSL (`wsl`) or Git Bash, not from `cmd.exe`.
- If bash reports `$'\r': command not found`, the file has Windows line
  endings. Fix it once with:
  ```bash
  sed -i 's/\r$//' *.sh
  ```

## A few conventions used here

- `#!/bin/bash` on the first line selects the interpreter.
- `[[ ... ]]` is used for bash-specific tests; `[ ... ]` for POSIX-style ones.
- Interactive `read` prompts are guarded with `[ -t 0 ]`, so the scripts also
  work when input is redirected (`./run_all.sh` does exactly that).
- Temporary files are created with `mktemp` and removed by a `trap ... EXIT`.
- Exit status `0` means success; anything else means failure.

An older single-file demo also exists at
`CPU Scheduling Non-Preemptive/linux_shell_scripts_demo.sh`.
