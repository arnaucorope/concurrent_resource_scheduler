# Concurrent Resource Scheduler

A multithreaded simulation in C that coordinates access to shared resources using POSIX threads, mutexes, and FIFO or Earliest Deadline First scheduling.

Each worker, called a *coder*, cycles through compiling, debugging, and refactoring. Compiling requires two neighboring USB dongles, shared with other coders in a ring. Released dongles enter a configurable cooldown before they can be reused.

A referee thread monitors progress. The simulation ends when a coder misses its burnout deadline or every coder has completed the requested number of compilations. The executable retains its original name, `codexion`.

## Build and run

Requires `make` and a C compiler with POSIX threads support.

```bash
make
./codexion 5 3000 100 100 100 3 50 edf
```

The example starts five coders with a 3,000 ms burnout limit, 100 ms for each work phase, a target of three compilations per coder, and a 50 ms dongle cooldown.

```text
./codexion <coders> <burnout_ms> <compile_ms> <debug_ms> <refactor_ms> <compiles_required> <cooldown_ms> <fifo|edf>
```

| Argument | Meaning |
| --- | --- |
| `coders` | Number of worker threads and shared dongles; must be at least 1 |
| `burnout_ms` | Maximum elapsed time since a coder's last compilation start |
| `compile_ms` | Compilation duration while holding both dongles |
| `debug_ms` | Debugging duration after releasing the dongles |
| `refactor_ms` | Refactoring duration before requesting them again |
| `compiles_required` | Minimum completed compilations per coder before normal termination |
| `cooldown_ms` | Delay before a released dongle becomes available for reuse |
| `fifo` / `edf` | Resource scheduling policy |

Numeric arguments are non-negative integers up to `INT_MAX`, except `coders`, which must be positive. A compilation target of zero exits without starting the worker threads.

Output lines contain elapsed milliseconds, a coder ID, and an event such as `is compiling` or `burned out`.

The Makefile compiles with `-Wall -Wextra -Werror -pthread`. Use `make clean` to remove object files, `make fclean` to also remove the executable, and `make re` to rebuild.

## Resource scheduling

Each dongle maintains a small priority queue for its neighboring coders. A coder registers a request with both dongles and can acquire them only when it:

- heads both queues;
- finds both dongles available;
- finds both cooldown periods complete.

Availability is checked and updated while holding both dongle mutexes. A coder therefore acquires the pair together instead of holding one resource while waiting for the other.

| Policy | Request priority |
| --- | --- |
| FIFO | Lowest request sequence number first; sequence numbers are assigned under a shared mutex |
| EDF | Earliest deadline first, with coder ID as the tie-breaker |

The EDF deadline is computed when the request is created:

```text
deadline = last_compile_start + time_to_burnout
```

After compilation, both resources are released with `cooldown_until = release_time + dongle_cooldown`. Waiting coders retry acquisition with a short sleep between attempts.

## Synchronization and lifecycle

### Coordinated startup

Workers and the referee wait on a condition variable. Once all threads have been created, the simulation publishes a common start time and wakes them with `pthread_cond_broadcast()`. Odd-numbered coders delay their first attempt by 2 ms to reduce initial contention.

### Lock ordering and shared state

Whenever two dongle mutexes are needed, they are locked in ascending dongle-ID order and unlocked in reverse order. This removes circular waiting between those mutexes.

Synchronization is divided by responsibility:

| Mutex | Protected state |
| --- | --- |
| Per-dongle mutex | Availability, cooldown, and waiting requests |
| Per-coder `state_mutex` | Last compilation start and completed compilation count |
| `request_mutex` | FIFO request counter |
| `start_mutex` | Startup state and condition-variable coordination |
| `stop_mutex` | Simulation termination flag |
| `print_mutex` | Serialized event output |

### Monitoring and shutdown

The referee checks each coder's last compilation start and completion count under that coder's state mutex. It sets the shared stop flag when a deadline is reached or all coders meet the compilation target.

Termination is cooperative: workers check the stop state during resource acquisition and timed waits. The main thread joins the workers and referee before cleanup. A coder can exceed its individual compilation target while waiting for the others to finish.

## Timing and boundary cases

- With one coder and a nonzero compilation target, only one dongle exists, so compilation cannot start and the coder eventually burns out.
- Queue priorities make resource arbitration explicit; they do not guarantee that every timing configuration can meet its deadlines.
- The referee and resource-acquisition loops use polling sleeps. Actual wake-up times depend on operating-system scheduling, so this is not a hard real-time scheduler.
- Tight deadlines, resource cooldowns, and contention can cause burnout even when mutex acquisition itself is deadlock-free.

## Code organization

| Location | Responsibility |
| --- | --- |
| `src/parse/` | Argument validation and conversion |
| `src/init/` | Simulation data and synchronization initialization |
| `src/simulation/coder.c` | Worker lifecycle and event logging |
| `src/simulation/dongle*.c`, `src/simulation/heap.c` | Resource requests, priorities, acquisition, and release |
| `src/simulation/referee.c` | Deadline and completion monitoring |
| `src/simulation/simulation.c` | Thread creation, startup, and joining |
| `src/utils/` | Timing, lock-order helpers, and cleanup |

## Project background

Developed by **Arnau Corominas Pérez (acoromin)** as **Codexion**, part of the 42 curriculum.

Reference material includes POSIX threads documentation and the manual pages for `pthread_create`, `pthread_join`, mutexes, condition variables, `gettimeofday`, and `usleep`.

AI tools supported concept exploration, design discussions, code review, and stress-test design. Suggestions were reviewed, tested, and adapted during development.
