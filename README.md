
# 🐧 System Programming & OS Internals in C

A collection of Linux-based C projects designed to explore how operating systems work under the hood, covering processes, threads, CPU scheduling, and systems-level performance analysis.

## 📂 Projects

### 1. Threads vs. Processes Benchmark

A benchmark comparing POSIX threads and processes while performing the same computational workload.

The program generates a large array of random integers, divides the work among multiple workers, and computes the overall average. By running the same workload with both threads and processes, the project highlights the performance trade-offs between shared-memory and isolated-memory execution models.

#### Key Objectives

- Compare thread and process creation overhead
- Measure execution-time differences
- Observe memory sharing and isolation behavior
- Explore OS resource limitations at large scales

#### Configuration

The total workload size is controlled by:

```c
#define ARRAY_SIZE 600000
```

and can be modified before compilation.

The number of workers (`N`) is provided dynamically as a command-line argument.

#### Build & Run

**Threads**

```bash
gcc assignment_threads.c -pthread -Wall -o threads_app
time ./threads_app 100
```

**Processes**

```bash
gcc assignment_processes.c -Wall -o processes_app
time ./processes_app 100
```

Typical worker counts tested:

```text
1, 10, 100, 1,000, 10,000, 100,000, 200,000
```

## 2. Multi-Level Queue CPU Scheduler

This project is a custom-built simulator of a single-processor CPU scheduler. It acts like the brain of an operating system, deciding which process receives CPU time, in what order, and for how long.

### Core Features

- **Multi-Level Priority Scheduling**
  - Maintains separate ready queues for different priority classes (**A > B > C**).
  - Higher-priority processes immediately preempt lower-priority ones.

- **Time Quantum Support**
  - Each priority level has its own execution quantum:
    - **A = 2**
    - **B = 4**
    - **C = 8**
  - Simulates time-sliced CPU sharing similar to real operating systems.

- **Dynamic Memory Management**
  - Processes are dynamically allocated using `malloc()` when they arrive.
  - Completed processes are properly released using `free()`.
  - Designed to prevent memory leaks during long-running simulations.

---
### 📊 Understanding the Test Cases & Output

To verify scheduler behavior, the simulator reads text files (e.g., `case3.txt`) that describe process arrivals over time.

#### Input Format

Each line in a test file contains:

- Arrival Time
- Priority Type (`A`, `B`, or `C`)
- Process ID
- Required Burst Time

#### `PRINTALL` Command

Some test cases contain a special `PRINTALL` command.

When the simulator reaches a time step containing `PRINTALL`, it prints a snapshot of the current scheduler state.

#### Reading the Output

Each snapshot contains three sections:

- **RP (Running Process)** The process currently executing on the CPU.

- **WP (Waiting Processes)** Processes waiting in the ready queues.

- **FP (Finished Processes)** Processes that have completed execution.

By tracing these snapshots, you can verify that the scheduler correctly:

- Handles process preemption
- Enforces queue-specific time quanta
- Prioritizes higher-priority tasks
- Manages process lifecycles and memory safely

---

### 🚀 Compile and Run

This project requires a standard Linux environment (or a Docker container).

#### 1. Compile the Scheduler

```bash
gcc -std=c11 -Wall -Werror \
    main.c scheduler.c cpu.c queue.c process.c input.c \
    -o scheduler
```

#### 2. Run a Simulation

```bash
./scheduler ./test/case3.txt
```

#### 3. Verify Memory Safety (Optional)

```bash
valgrind --track-origins=yes --leak-check=full \
    ./scheduler ./test/case3.txt
```
