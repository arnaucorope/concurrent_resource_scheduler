*This project has been created as part of the 42 curriculum by acoromin.*

# Codexion

## Description

Codexion is a multithreaded simulation written in C with POSIX threads.

Each coder is represented by its own thread and repeatedly alternates between compiling, debugging, and refactoring. To compile, a coder must obtain its two neighboring USB dongles. Dongles are shared resources, have a cooldown after being released, and arbitrate competing requests according to one of two scheduling policies:

- `fifo`: oldest request first.
- `edf`: earliest burnout deadline first.

A separate referee thread monitors coder deadlines and the required number of compiles. The simulation stops when a coder burns out or when every coder reaches the requested compile count.

The project focuses on concurrency, synchronization, fair resource arbitration, deadlock prevention, starvation avoidance, timing precision, and safe cleanup.

## Instructions

### Compilation

```bash
make
```

The project is compiled with:

```text
-Wall -Wextra -Werror -pthread
```

Available Makefile rules:

```bash
make
make clean
make fclean
make re
```

### Execution

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

All time values are expressed in milliseconds. The scheduler must be exactly `fifo` or `edf`.

Example:

```bash
./codexion 5 3000 100 100 100 3 50 edf
```

## Scheduling

### FIFO

FIFO orders requests by creation order. Each request receives a monotonically increasing `order` value protected by `request_mutex`.

The request with the lowest order value has the highest priority.

### EDF

EDF means Earliest Deadline First. The request deadline is calculated as:

```text
deadline = last_compile_start + time_to_burnout
```

The earliest deadline has the highest priority. If two deadlines are equal, the coder ID is used as a deterministic tie-breaker.

Each dongle owns a priority queue containing the coders currently waiting for it.

### Initial startup staggering

To reduce contention when all coder threads start simultaneously, odd-numbered
coders wait briefly before their first scheduling attempt.

This gives non-adjacent even-numbered coders a small head start when registering
their initial requests, reducing contention for shared dongles.

The stagger is applied only once at startup. After that, all coders follow the
normal FIFO or EDF scheduling policy.

## Blocking cases handled

### Deadlock prevention

A coder needs two shared dongles to compile. If different threads locked dongles in inconsistent orders, a circular wait could occur.

Whenever both dongle mutexes must be locked, Codexion always locks them in deterministic dongle-ID order:

```text
lower ID -> higher ID
```

Because every coder follows the same ordering, the circular-wait Coffman condition is removed and a mutex deadlock between neighboring coders is avoided.

### Starvation prevention and fair arbitration

Dongles are not granted simply to whichever thread happens to run first.

Each dongle maintains a priority queue. Requests are ordered according to the selected scheduler:

```text
FIFO -> oldest request first
EDF  -> earliest burnout deadline first
```

For equal EDF deadlines, coder ID provides a deterministic tie-breaker.

This gives competing coders an explicit and reproducible arbitration policy instead of relying on operating-system thread scheduling alone.

### Dongle cooldown

After release, a dongle cannot immediately be reused.

Each dongle stores a timestamp:

```text
cooldown_until = release_time + dongle_cooldown
```

A coder may use the dongle only when the current time has reached that timestamp.

### Precise burnout detection

Burnout is monitored by a dedicated referee thread.

Each coder stores `last_compile_start`. The referee reads this value under the coder's state mutex and compares it with:

```text
last_compile_start + time_to_burnout
```

When a deadline is reached, the referee sets the global stop state and prints the burnout event.

### Log serialization

All simulation output is protected by `print_mutex` so that two threads cannot interleave characters on the same output line.

Normal coder logs also check the synchronized stop state before printing, preventing regular status messages after the simulation has ended.

### Single-coder case

With one coder there is only one dongle. Since compiling requires two dongles simultaneously, the coder cannot compile and eventually burns out.

## Thread synchronization mechanisms

Codexion uses `pthread_mutex_t` and `pthread_cond_t` to protect shared state and coordinate thread execution.

### Dongle mutexes

Each dongle owns a mutex protecting:

- availability;
- cooldown state;
- its waiting priority queue.

Any thread that reads or modifies those fields does so while holding the corresponding dongle mutex.

When an operation needs both neighboring dongles, the mutexes are locked in deterministic ID order to prevent circular wait.

### Coder state mutexes

Each coder owns a `state_mutex` protecting state shared with the referee, including:

```text
last_compile_start
compiles_done
```

The coder thread locks this mutex when updating its state, and the referee locks the same mutex when reading it. This prevents data races between the worker and monitor threads.

### Global synchronization mutexes

Codexion also uses dedicated mutexes for independent shared concerns:

- `print_mutex`: serializes log output.
- `stop_mutex`: protects the global simulation stop state.
- `request_mutex`: protects the FIFO request counter.
- `start_mutex`: protects the synchronized simulation start state.

Separating these responsibilities avoids using one large global lock and reduces unnecessary contention.

### Start condition variable

Coder threads are created before the official simulation start and wait on a condition variable.

The start state is protected by `start_mutex`. Once initialization is complete, the program publishes the common start time and wakes waiting threads with `pthread_cond_broadcast()`.

This ensures that all coders and the referee share a consistent simulation starting point.

### Referee communication

The referee does not cancel coder threads directly.

Instead, it communicates through synchronized shared state:

```text
referee detects burnout/completion
        |
        v
sets simulation_over
        |
        v
coder threads observe the stop state and exit
```

This keeps termination cooperative and thread-safe.

No custom event abstraction is used; synchronization is implemented directly with POSIX mutexes, condition variables, and protected shared state.

## Resources

Useful references for this project include:

- POSIX threads documentation.
- Linux manual pages for:
  - `pthread_create(3)`
  - `pthread_join(3)`
  - `pthread_mutex_init(3)`
  - `pthread_mutex_lock(3)`
  - `pthread_mutex_unlock(3)`
  - `pthread_mutex_destroy(3)`
  - `pthread_cond_init(3)`
  - `pthread_cond_wait(3)`
  - `pthread_cond_broadcast(3)`
  - `pthread_cond_destroy(3)`
  - `gettimeofday(2)`
  - `usleep(3)`
- Material about race conditions, deadlocks, Coffman's conditions, priority queues, FIFO scheduling, and Earliest Deadline First scheduling.

### Use of AI

AI was used as a learning and review tool during development, mainly for:

- understanding POSIX threads, mutexes, and condition variables;
- discussing FIFO and EDF scheduling;
- reasoning about priority queues and shared-resource arbitration;
- reviewing possible deadlocks, race conditions, and starvation cases;
- reviewing initialization, cleanup, cooldown, and burnout logic;
- designing stress tests and discussing observed concurrency behavior.

The generated suggestions were reviewed, tested, and adapted before being integrated into the project.
