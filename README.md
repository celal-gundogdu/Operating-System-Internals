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

---
### 2. Multi-Level Queue CPU Scheduler

A custom CPU scheduling simulator that models how an operating system selects which process should run and for how long.

The scheduler maintains multiple priority queues, supports time slicing through configurable quantums, and performs preemption whenever a higher-priority process becomes ready. The project provides a simplified but practical view of how modern operating systems manage CPU resources.

#### Features

- Multi-Level Queue Scheduling
- Priority-Based Process Selection
- FIFO Queue Management
- Configurable Time Quantums
- Preemptive Context Switching
- Dynamic Process Creation and Removal
- Tick-Based Scheduling Simulation

#### Build & Run

```bash
gcc *.c -Wall -o scheduler
./scheduler case1.txt
```

---
## 🛠 Technologies
- **C** — systems-level programming
- **POSIX Threads (pthreads)** — multithreading
- **Linux System Calls (`fork`)** — process creation
- **Dynamic Memory Allocation**
- **Linked Lists & FIFO Queues**
- **CPU Scheduling Algorithms**
- **Performance Benchmarking**
- **Linux Systems Programming**
