OS Lab C Programs
==================

Compile on Linux (gcc). Programs using semaphores or shared memory require pthread or librt:

CPU Scheduling (Preemptive & Non-Preemptive):
  gcc non_preemptive_fcfs.c        -o non_preemptive_fcfs
  gcc non_preemptive_sjf.c         -o non_preemptive_sjf
  gcc non_preemptive_priority.c    -o non_preemptive_priority
  gcc preemptive_srtf.c            -o preemptive_srtf
  gcc preemptive_priority.c        -o preemptive_priority
  gcc preemptive_round_robin.c     -o preemptive_round_robin

Memory Allocation:
  gcc first_fit.c                  -o first_fit
  gcc best_fit.c                   -o best_fit
  gcc worst_fit.c                  -o worst_fit

Page Replacement:
  gcc fifo_page_replacement.c      -o fifo
  gcc lru_page_replacement.c       -o lru
  gcc optimal_page_replacement.c   -o optimal

Process Synchronization & Concurrency:
  gcc dining_philosophers.c        -o dining_philosophers -pthread
  gcc producer_consumer.c          -o producer_consumer -pthread
  gcc readers_writers.c            -o readers_writers -pthread
  gcc producer_consumer_pipe.c     -o producer_consumer_pipe -pthread
  gcc readers_writers_process.c    -o readers_writers_process -pthread
  gcc shared_memory.c              -o shared_memory -lrt

Deadlock Avoidance:
  gcc bankers_algorithm.c          -o bankers

Process Creation & Exec:
  gcc process_creation.c           -o process_creation
  gcc two_child_processes.c        -o two_child_processes
  gcc orphan_zombie.c              -o orphan_zombie
  gcc print_even_numbers.c         -o print_even_numbers
  gcc child_exec_even.c            -o child_exec_even

Disk Scheduling:
  gcc fcfs_disk_scheduling.c       -o fcfs_disk_scheduling
  gcc sstf_disk_scheduling.c       -o sstf_disk_scheduling
  gcc scan_disk_scheduling.c       -o scan_disk_scheduling
  gcc cscan_disk_scheduling.c      -o cscan_disk_scheduling
  gcc look_disk_scheduling.c       -o look_disk_scheduling
  gcc clook_disk_scheduling.c      -o clook_disk_scheduling
  (run these from the "Disk Scheduling" folder)

Thread Creation:
  gcc thread_creation.c            -o thread_creation -pthread

Linux Shell Scripts (basic lessons are in the "Shell Scripts" folder):
  cd "Shell Scripts"
  chmod +x *.sh
  ./run_all.sh            (or run a single lesson, e.g. ./04_conditions.sh)

  Older single-file demo: CPU Scheduling Non-Preemptive/linux_shell_scripts_demo.sh

File System:
  gcc file_system_calls.c          -o file_system_calls

Full syllabus coverage map: SYLLABUS_COVERAGE.md
