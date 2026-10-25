# OS Lab Syllabus Coverage

Mapping of the 12 lab requirements to the files in this repository.

| # | Requirement | Files |
| --- | --- | --- |
| 1 | Linux commands and shell scripting | `Shell Scripts/01_hello_world.sh` … `15_linux_commands.sh`, `Shell Scripts/README.md`, `CPU Scheduling Non-Preemptive/linux_shell_scripts_demo.sh` |
| 2 | Process creation with `fork()`, orphan and zombie processes | `process_creation.c`, `two_child_processes.c`, `orphan_zombie.c` |
| 3 | Thread creation | `thread_creation.c` |
| 4 | CPU scheduling: FCFS, SJF, Priority (non-preemptive) | `CPU Scheduling Non-Preemptive/non_preemptive_fcfs.c`, `non_preemptive_sjf.c`, `non_preemptive_priority.c` |
| 5 | CPU scheduling: Round Robin, SRTF, preemptive Priority | `CPU Scheduling Preemptive/preemptive_round_robin.c`, `preemptive_srtf.c`, `preemptive_priority.c` |
| 6 | Banker's Algorithm (safe state + resource request) | `bankers_algorithm.c` |
| 7 | Producer-Consumer / Readers-Writers with semaphores | `producer_consumer.c`, `readers_writers.c`, plus pipe/process variants `producer_consumer_pipe.c` and `readers_writers_process.c` |
| 8 | Dining Philosophers with semaphores | `dining_philosophers.c` |
| 9 | Memory allocation: First Fit, Best Fit, Worst Fit | `Memory Allocation/first_fit.c`, `best_fit.c`, `worst_fit.c` |
| 10 | Page replacement: FIFO, LRU, Optimal | `Page Replacement/fifo_page_replacement.c`, `lru_page_replacement.c`, `optimal_page_replacement.c` |
| 11 | Disk scheduling: FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK | `Disk Scheduling/fcfs_disk_scheduling.c`, `sstf_disk_scheduling.c`, `scan_disk_scheduling.c`, `cscan_disk_scheduling.c`, `look_disk_scheduling.c`, `clook_disk_scheduling.c` |
| 12 | File system operations using system calls | `file_system_calls.c` |

## Quick compile/run

All C programs are compiled with `gcc` on Linux/WSL. Programs using POSIX
semaphores or threads are linked with `-pthread`; `shared_memory.c` uses
`-lrt`. Exact commands are in `README.txt`.

The shell lessons run with:

```bash
cd "Shell Scripts"
chmod +x *.sh
./run_all.sh
```

## Notes

- Orphan/zombie demo: `./orphan_zombie` (or `./orphan_zombie zombie` /
  `./orphan_zombie orphan`).
- Disk scheduling programs are run from the `Disk Scheduling` folder and ask
  for the request list, initial head, and (where relevant) the highest
  cylinder number.
- `MASTER_COPY.md` contains the complete source and sample output for every C
  program.
