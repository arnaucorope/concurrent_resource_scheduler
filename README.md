*This project has been created as part of the 42 curriculum by acoro.*

# Codexion

## Description

Codexion is a multithreaded simulation project written in C using POSIX threads.

The project models a group of coders competing for shared dongles in order to compile their work before reaching a burnout deadline.

Each coder runs in its own thread and repeatedly performs the following cycle:

1. Waits until the required dongles can be acquired.
2. Compiles.
3. Releases the dongles.
4. Debugs.
5. Refactors.
6. Repeats the process while the simulation is active.

A separate referee thread monitors the simulation and stops it when:

- A coder reaches burnout.
- Every coder has completed at least the required number of compiles.

Dongle access is coordinated using a priority queue and one of two scheduling policies:

- `fifo`: requests are prioritized according to arrival order.
- `edf`: requests are prioritized according to the earliest burnout deadline.

The project focuses on thread synchronization, shared-resource arbitration, race-condition prevention, deadlock avoidance, scheduling, and precise timing.

## Features

- One POSIX thread per coder.
- Separate referee thread.
- FIFO scheduling.
- EDF scheduling.
- Priority queues for dongle requests.
- Per-dongle mutex protection.
- Dongle cooldown support.
- Burnout monitoring.
- Serialized logging.
- Synchronized simulation startup.
- Global simulation stop state.
- Interruptible waiting during coder actions.
- Safe initialization and cleanup of allocated resources and synchronization primitives.

## Instructions

### Compilation

Compile the project with:

```bash
make
```

The project is compiled using:

```text
-Wall -Wextra -Werror -pthread
```

To remove object files:

```bash
make clean
```

To remove object files and the executable:

```bash
make fclean
```

To rebuild the entire project:

```bash
make re
```

### Execution

Run Codexion using:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

The scheduler must be either:

```text
fifo
```

or:

```text
edf
```

Example:

```bash
./codexion 5 3000 100 100 100 3 50 fifo
```

Example using EDF:

```bash
./codexion 5 3000 100 100 100 3 50 edf
```

All time values are expressed in milliseconds.

## Usage examples

A normal FIFO execution:

```bash
./codexion 3 1000 100 100 100 2 50 fifo
```

Possible output:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
100 1 is debugging
150 3 has taken a dongle
150 3 has taken a dongle
150 3 is compiling
200 1 is refactoring
```

A simulation designed to trigger burnout:

```bash
./codexion 4 100 200 100 100 10 100 fifo
```

Possible result:

```text
0 4 has taken a dongle
0 4 has taken a dongle
0 4 is compiling
100 1 burned out
```

When burnout is detected, the global simulation stop state is activated and normal coder status messages are no longer printed.

## Scheduling

### FIFO

FIFO prioritizes requests according to the order in which they were created.

Each new request receives a monotonically increasing value:

```text
request.order
```

The request with the lowest order value has the highest FIFO priority.

The global request counter is protected by a mutex to prevent multiple coder threads from modifying it simultaneously.

### EDF

EDF stands for Earliest Deadline First.

Each request stores a deadline calculated as:

```text
deadline = last_compile_start + time_to_burnout
```

The coder with the lowest deadline receives higher priority.

When two EDF deadlines are equal, the coder ID is used as a deterministic tie-breaker.

## Dongle management

Each dongle contains:

- An identifier.
- An availability state.
- A cooldown deadline.
- Its own mutex.
- A priority queue containing waiting coders.

A coder requires two dongles before it can compile.

When a coder requests access, it is registered in the waiting queues of the required dongles.

A coder may acquire its dongles only if:

- It has the highest priority in both dongle queues.
- Both dongles are available.
- Both dongles have completed their cooldown period.

Once acquired, both dongles are marked unavailable.

After compilation finishes, the dongles are released and their cooldown deadline is updated:

```text
cooldown_until = current_time + dongle_cooldown
```

The cooldown is handled using timestamps rather than by creating additional timer threads.

## Blocking cases handled

### Deadlock prevention

Several coders may compete for overlapping dongles.

A classical deadlock could occur if each coder locked one dongle and then waited indefinitely for another dongle held by a different coder.

For example:

```text
Coder 1 owns D1 and waits for D2
Coder 2 owns D2 and waits for D3
Coder 3 owns D3 and waits for D4
Coder 4 owns D4 and waits for D1
```

This creates a circular wait.

To prevent this, dongle mutexes are always acquired according to a global order based on dongle ID.

For two required dongles:

```text
lower dongle ID
then
higher dongle ID
```

All coder threads therefore follow the same lock ordering.

This removes the circular-wait condition required for a deadlock to occur.

This solution directly addresses one of Coffman's deadlock conditions: circular wait.

### Starvation prevention

Requests are not granted simply according to whichever thread happens to run first.

Each dongle maintains a priority queue.

The selected scheduling policy determines request priority:

```text
FIFO -> oldest request first
EDF  -> earliest burnout deadline first
```

This provides deterministic arbitration between competing coders.

EDF additionally favors coders that are closer to their burnout deadline.

### Dongle cooldown

A dongle cannot immediately be reused after being released.

Each dongle stores:

```text
cooldown_until
```

When released:

```text
cooldown_until = now + dongle_cooldown
```

A waiting coder is allowed to acquire the dongle only when:

```text
current_time >= cooldown_until
```

This avoids blocking an entire thread solely for the purpose of waiting for a cooldown timer.

### Burnout detection

Burnout monitoring is performed by a dedicated referee thread.

Each coder stores:

```text
last_compile_start
```

The referee periodically compares the current time with the coder's burnout deadline.

Conceptually:

```text
deadline = last_compile_start + time_to_burnout
```

If the current time reaches or exceeds that deadline, the referee stops the simulation and prints the burnout event.

The referee checks the simulation frequently in order to provide precise burnout detection.

### Interruptible coder waiting

A coder must remain responsive to the global stop state while compiling, debugging, or refactoring.

Instead of sleeping for the complete action duration with one long `usleep`, Codexion uses a helper that sleeps in short intervals.

Conceptually:

```text
sleep briefly
check simulation state
sleep briefly
check again
...
```

This allows coder threads to stop shortly after the referee ends the simulation.

### Log serialization

Multiple coder threads may attempt to print simultaneously.

All normal log output is protected using:

```c
pthread_mutex_t print_mutex;
```

Only one thread can therefore execute the protected printing section at a time.

This prevents output from different threads from being interleaved.

Normal status logging also checks whether the simulation has already ended before printing.

The burnout message is handled separately by the referee so that it can still be printed while the global stop state is active.

### Single-coder case

With one coder, only one dongle exists.

Since compiling requires two dongles, the coder cannot compile and eventually reaches burnout.

## Thread synchronization mechanisms

Codexion uses POSIX synchronization primitives to coordinate access to shared state.

### `pthread_mutex_t`

Several mutexes are used for different responsibilities.

#### Dongle mutex

Each dongle owns its own mutex.

It protects:

- Dongle availability.
- Dongle cooldown state.
- The dongle waiting queue.

When operations require both dongles, their mutexes are acquired in deterministic ID order to prevent deadlocks.

#### `state_mutex`

Each coder has a state mutex.

It protects mutable coder state shared with the referee, including:

```text
last_compile_start
compiles_done
```

For example, when a coder starts compiling:

```text
lock state_mutex
update last_compile_start
unlock state_mutex
```

The referee uses the same mutex when reading the value.

This prevents a data race between the coder thread and referee thread.

#### `print_mutex`

The print mutex serializes terminal output.

Only one thread is allowed to print a simulation event at a time.

#### `stop_mutex`

The global:

```text
simulation_over
```

state is protected by a mutex.

All threads use synchronized helper functions when reading or modifying the stop state.

This provides thread-safe communication between the referee and coder threads.

#### `request_mutex`

FIFO requests require a global monotonically increasing request counter.

The counter is protected using:

```c
pthread_mutex_t request_mutex;
```

This prevents two threads from receiving an inconsistent or duplicated request order.

#### `start_mutex`

The simulation start state is protected using:

```c
pthread_mutex_t start_mutex;
```

It is used together with the start condition variable.

### `pthread_cond_t`

Codexion uses a condition variable to synchronize the beginning of the simulation.

All coder threads are created before the simulation officially begins.

They wait on:

```c
pthread_cond_wait()
```

until the simulation start state is ready.

When initialization has finished, the simulation sets the start state and wakes all waiting threads using:

```c
pthread_cond_broadcast()
```

While waiting on the condition variable, `pthread_cond_wait()` temporarily releases the associated mutex.

This allows the thread responsible for starting the simulation to acquire the mutex, update the start state, and wake the waiting threads.

This ensures that every coder and the referee use a consistent simulation starting point.

### Referee communication

The referee does not directly terminate coder threads.

Instead, communication occurs through shared synchronized state.

The referee:

```text
reads coder state
        |
        v
detects burnout or completion
        |
        v
sets simulation_over
```

Coder threads periodically read the same synchronized stop state and terminate their execution when the simulation ends.

This provides thread-safe communication without requiring thread cancellation.

## Project flow

```text
main
|
+-- parse arguments
|
+-- convert arguments
|
+-- initialize simulation
|
+-- run simulation
|   |
|   +-- create coder threads
|   |
|   +-- create referee thread
|   |
|   +-- synchronize simulation start
|   |
|   +-- coder threads
|   |   |
|   |   +-- request dongles
|   |   +-- acquire dongles
|   |   +-- compile
|   |   +-- release dongles
|   |   +-- debug
|   |   +-- refactor
|   |
|   +-- referee
|       |
|       +-- check burnout
|       +-- check compile completion
|
+-- join threads
|
+-- cleanup resources
```

## Cleanup

All dynamically allocated memory and initialized synchronization objects are released when they are no longer needed.

The cleanup process includes:

- Destroying coder state mutexes.
- Freeing the coder array.
- Destroying dongle mutexes.
- Freeing dongle waiting queues.
- Freeing the dongle array.
- Destroying the print mutex.
- Destroying the stop mutex.
- Destroying the request mutex.
- Destroying the start mutex.
- Destroying the start condition variable.

Partial cleanup paths are also used during initialization.

If initialization fails after only some resources have been created, only successfully initialized resources are destroyed.

This avoids:

- Memory leaks.
- Double free operations.
- Destroying uninitialized mutexes.
- Destroying the same synchronization primitive more than once.

## Resources

The following resources were useful for understanding the concepts required by this project:

- POSIX Threads documentation.
- Linux manual pages:
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
- Documentation and educational material about:
  - POSIX threads.
  - Race conditions.
  - Mutex synchronization.
  - Condition variables.
  - Deadlocks.
  - Coffman's deadlock conditions.
  - Priority queues and binary heaps.
  - FIFO scheduling.
  - Earliest Deadline First scheduling.
  - Concurrent resource arbitration.

### Use of AI

AI was used as a learning, reasoning, and review assistant during the development of the project.

It was used for:

- Understanding POSIX thread behavior.
- Understanding mutexes and condition variables.
- Reasoning about thread synchronization.
- Discussing the structure of the coder/referee model.
- Designing FIFO and EDF request arbitration.
- Understanding priority queues and heap-based scheduling.
- Identifying possible deadlock scenarios.
- Reasoning about deterministic lock ordering.
- Reviewing race-condition scenarios.
- Reviewing initialization and cleanup paths.
- Discussing timestamp-based dongle cooldown handling.
- Designing burnout and stress test cases.
- Reviewing compiler errors and integration issues.
- Explaining individual sections of the implementation during development.

AI was primarily used to explain concepts, discuss possible designs, and review code incrementally.

The implementation was integrated, tested, and adapted as part of the development process.
