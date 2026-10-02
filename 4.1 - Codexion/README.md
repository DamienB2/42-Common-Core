*This project has been created as part of the 42 curriculum by dbaise.*
 
# Codexion
 
> Master the race for resources before the deadline masters you.
 
## Description
 
**Codexion** is a concurrency simulation written in C with POSIX threads. A group of coders sits around a circular table, with one USB dongle between each pair of neighbours (as many dongles as coders). To compile their quantum code, a coder must hold **two dongles at the same time**: the one on their left and the one on their right. After compiling, they put both dongles back, debug, then refactor, and immediately try to compile again.
 
If a coder does not **start** a compile within `time_to_burnout` ms of the start of their previous compile (or of the simulation), they burn out and the simulation stops. The simulation also stops when every coder has compiled at least `number_of_compiles_required` times.
 
The goals of the project are to:
 
- share limited resources between threads without data races or deadlocks,
- guarantee fair access to those resources (FIFO or EDF arbitration),
- detect burnout precisely from a dedicated monitor thread,
- keep the output readable (no interleaved log lines) and the memory clean (no leaks).
### Overview of the implementation
 
| File | Role |
|---|---|
| `codexion.c` | `main`: parse arguments, run the simulation |
| `parser.c`, `parser_helper.c` | Argument validation and conversion (`ft_atoi`, `ft_is_str_digit`) |
| `simulation.c` | Thread creation, joining and cleanup |
| `sim_init.c` | Initialization of the shared state, dongles and coders |
| `coder.c`, `coder_actions.c` | Coder thread routine and the compile / debug / refactor actions |
| `dongle.c`, `dongle_init.c` | Dongle logic: take, release, cooldown, waiting queue |
| `priority_queue.c` | Priority queue used for FIFO / EDF arbitration |
| `monitor.c` | Monitor thread: burnout detection |
| `sim_utils.c`, `sim_utils_2.c` | Stop flag, logging, per-coder counters |
| `time_helper.c` | Time helpers based on `gettimeofday` |
| `codexion.h` | Structures and prototypes |
 
## Instructions
 
### Compilation
 
```bash
make        # builds ./codexion with cc -Wall -Wextra -Werror -g -pthread
make clean  # removes the object files
make fclean # removes the object files and the binary
make re     # fclean + all
```
 
### Execution
 
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
 
All eight arguments are mandatory.
 
| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and of dongles). Must be > 0. |
| `time_to_burnout` | Burnout delay in ms, counted from the start of the last compile (or of the simulation). Must be > 0. |
| `time_to_compile` | Duration of a compile in ms (two dongles held). Must be > 0. |
| `time_to_debug` | Duration of the debugging phase in ms. Must be > 0. |
| `time_to_refactor` | Duration of the refactoring phase in ms. Must be > 0. |
| `number_of_compiles_required` | The simulation stops when every coder has compiled at least this many times. With `0`, the program exits immediately without output. |
| `dongle_cooldown` | Time in ms during which a released dongle cannot be taken again. Can be `0`. |
| `scheduler` | `fifo` or `edf` (exactly). |
 
Invalid input (wrong number of arguments, negative or non-numeric values, values overflowing an `int`, unknown scheduler) is rejected with an error message.
 
### Example
 
```bash
./codexion 5 800 200 200 200 7 0 fifo
```
 
### Output format
 
Every state change is printed on its own line:
 
```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```
 
The timestamp is the number of milliseconds elapsed since the start of the simulation, and `X` is the coder number (from `1` to `number_of_coders`).
 
## Blocking cases handled
 
### Deadlock prevention (Coffman's conditions)
 
A deadlock requires four conditions to hold simultaneously: mutual exclusion, hold and wait, no preemption, and circular wait. Mutual exclusion (a dongle is held by one coder at a time) and the fact that dongles cannot be taken away from a coder are inherent to the problem, so the solution breaks the **circular wait** condition.
 
Coders do not all take their dongles in the same order. In `get_dongles_order` (`coder.c`):
 
- coders with an **odd** id take their **left** dongle first, then the right one;
- coders with an **even** id take their **right** dongle first, then the left one.
Neighbouring coders therefore do not all hold their "first" dongle while waiting for the "second" one held by the next coder, so the wait-for graph cannot close into a cycle. In addition, if a coder fails to obtain the second dongle (the simulation is stopping), they release the first one before leaving, so no dongle stays locked.
 
With a single coder, there is only one dongle on the table (left and right are the same dongle). The coder can never get two dongles, cannot compile and burns out after `time_to_burnout` ms, detected by the monitor like any other burnout.
 
### Starvation prevention (fair arbitration)
 
Each dongle owns a waiting queue (`t_queue`) protected by the dongle's mutex. A coder pushes a request into the queue of the dongle, and only the coder at the **head of the queue** may take the dongle (`dongle_ready`).
 
- **FIFO**: priority is given to the smallest arrival number (`seq`), a per-dongle counter incremented under the dongle's lock.
- **EDF** (Earliest Deadline First): priority is given to the earliest deadline, with `deadline = last_compile_start + time_to_burnout`. The deadline is computed when the coder starts trying to acquire their dongles. Equal deadlines are broken by the arrival number `seq`, which makes the policy fully deterministic.
The comparison is implemented in `pq_higher_priority` (`priority_queue.c`). Since a dongle is shared by only two neighbouring coders, at most two requests can be waiting on it at the same time, so the queue has a fixed capacity of two nodes.
 
### Cooldown handling
 
When a coder releases a dongle, `release_dongle` stores `free_at = now + dongle_cooldown` and clears the `taken` flag. A dongle is only granted when it is not taken **and** `now >= free_at`.
 
Nobody signals the end of a cooldown. A coder that is at the head of the queue while the dongle is free but still cooling down therefore releases the dongle's mutex, sleeps 500 µs, then re-checks the condition. In every other case (dongle taken, or not first in the queue) the coder waits on `pthread_cond_wait` and is woken by a broadcast. A release also broadcasts the condition variable so that waiters can react immediately when the cooldown is `0`.
 
### Precise burnout detection
 
A dedicated **monitor thread** (`monitor_routine`) loops every 1 ms. Under `counters_lock`, it checks for each coder whether `now >= last_compile_start + time_to_burnout`. The coder threads update `last_compile_start` at the very beginning of each compile.
 
When a burnout is found, `print_death` sets the stop flag and prints the `burned out` line while holding the print lock, then wakes up all the dongles so that blocked coders leave their waiting loop. Because of the 1 ms polling period, the burnout message is printed well within the 10 ms tolerance required by the subject.
 
### Log serialization
 
All output goes through `print_state` / `print_death`, which lock `print_lock` for the whole `printf`, so two messages can never be mixed on one line. The stop flag is checked **inside** the print lock: once the simulation is stopped (burnout or all compiles done), no coder can print anything else, so the `burned out` line is always the last line of the output.
 
### Clean shutdown
 
- When every coder has reached `number_of_compiles_required`, the coder that completes the last compile sets the stop flag and wakes all dongles (`finish_compile`).
- Coders blocked in `take_dongle` leave their waiting loop when they see the stop flag, and remove themselves from the queue.
- The main thread joins all the coder threads, sets the stop flag again, wakes the dongles, joins the monitor thread, then destroys all mutexes and condition variables and frees all allocated memory (`cleanup_simulation`).
## Thread synchronization mechanisms
 
### Primitives used
 
| Primitive | Where | What it protects |
|---|---|---|
| `pthread_t` (one per coder + one monitor) | `t_coder.thread`, `t_sim.monitor_thread` | Each coder is a thread created with `pthread_create`, plus one monitor thread |
| `pthread_mutex_t lock` (one per dongle) | `t_dongle` | `taken`, `free_at`, `next_seq` and the waiting queue of the dongle |
| `pthread_cond_t cond` (one per dongle) | `t_dongle` | Waiting for a dongle to become available (release, cooldown, stop) |
| `pthread_mutex_t print_lock` | `t_sim` | The standard output (one line at a time) |
| `pthread_mutex_t stop_lock` | `t_sim` | The `stop` flag shared by all threads |
| `pthread_mutex_t counters_lock` | `t_sim` | `last_compile_start[]` and `nb_compiles_done[]` |
 
No global variable is used: everything is reached through the `t_sim` structure shared by pointer, and through each thread's own `t_coder`.
 
### How race conditions are prevented
 
- **A dongle can never be duplicated.** 
`take_dongle` locks the dongle, pushes the request, waits until `dongle_ready` is true, then sets `taken = 1` before unlocking. The test and the update happen inside the same critical section, so two coders cannot both see the dongle as free. While waiting, a coder uses `pthread_cond_wait` (woken up by the broadcasts of `release_dongle`, `take_dongle` and `wake_all_dongles`). The only case where nobody will send a signal is the end of a cooldown: a coder that is at the head of the queue, with the dongle free, then releases the mutex, sleeps 500 µs and re-checks the condition.
```c
  pthread_mutex_lock(&dongle->lock);
  pq_push(&dongle->queue, node);
  while (!dongle_ready(dongle, id) && !sim_should_stop(sim))
  {
      if (pq_peek_id(&dongle->queue) == id && !dongle->taken)
      {
          pthread_mutex_unlock(&dongle->lock);
          usleep(500);
          pthread_mutex_lock(&dongle->lock);
      }
      else
          pthread_cond_wait(&dongle->cond, &dongle->lock);
  }
  ...
  dongle->taken = 1;
  pthread_mutex_unlock(&dongle->lock);
```
 
- **The stop flag is never read or written without its mutex.** `sim_should_stop` and `sim_stop_simulation` are the only accessors of `sim->stop`.
- **Compile counters and timestamps are shared safely between coders and the monitor.** A coder writes `last_compile_start[i]` with `set_last_compile_start`, and the monitor reads it in `find_dead_coder`, both under `counters_lock`. The same mutex protects `nb_compiles_done`, read by `all_compiles_done`.
- **No lock-order inversion.** The only place where two mutexes are nested is the pair `print_lock` then `stop_lock` (in `print_state` and `print_death`). `stop_lock` is never held while waiting for `print_lock`, and dongle locks are never held while taking another lock of the simulation (except `stop_lock` for a very short read), so no cycle between mutexes is possible.
### Thread-safe communication between coders and the monitor
 
Coders and the monitor never call each other: they communicate **only through shared state protected by mutexes**.
 
- Coder → monitor: the coder publishes the start time of each compile (`counters_lock`).
- Monitor → coders: the monitor sets the stop flag (`stop_lock`) and broadcasts every dongle's condition variable, which wakes up any coder that is blocked waiting for a dongle. The coders then see the flag and exit their routine.
- Coder → everyone: the coder completing the last required compile sets the stop flag and wakes all the dongles in the same way.
## Tests and scenarios
 
The following scenarios can be run by hand to check the behaviour of the program. Times are in milliseconds.
 
### Argument validation
 
| Command | Expected result |
|---|---|
| `./codexion` | Error: wrong number of arguments |
| `./codexion 5 800 200 200 200 7 0` | Error: wrong number of arguments (scheduler missing) |
| `./codexion 5 800 200 200 200 7 0 lifo` | Error: scheduler must be `fifo` or `edf` |
| `./codexion -5 800 200 200 200 7 0 fifo` | Error: not a positive integer |
| `./codexion abc 800 200 200 200 7 0 fifo` | Error: not a positive integer |
| `./codexion 5 800 200 200 200 7 99999999999 fifo` | Error: value out of range |
| `./codexion 0 800 200 200 200 7 0 fifo` | Error: `number_of_coders` is incorrect |
| `./codexion 5 0 200 200 200 7 0 fifo` | Error: time values are incorrect |
| `./codexion 5 800 200 200 200 0 0 fifo` | Exits immediately, no output |
 
### Simulation behaviour
 
| Command | What to check |
|---|---|
| `./codexion 1 800 200 200 200 5 0 fifo` | A single dongle: the coder takes it once, never compiles, and `1 burned out` is printed about 800 ms after the start |
| `./codexion 5 800 200 200 200 7 0 fifo` | Feasible parameters: the simulation should end once every coder has compiled 7 times, without any burnout |
| `./codexion 5 800 200 200 200 7 0 edf` | Same parameters with EDF: compare the order in which dongles are granted with the FIFO run |
| `./codexion 4 310 200 100 100 10 0 fifo` | Tight parameters: a burnout is expected, check that its timestamp is within 10 ms of `last_compile_start + 310` |
| `./codexion 5 800 200 200 200 7 200 fifo` | With a cooldown: a dongle must never be taken less than 200 ms after its release |
| `./codexion 2 800 200 200 200 5 0 fifo` | Two coders sharing both dongles: no deadlock |
| `./codexion 200 2000 100 100 100 5 0 edf` | Many threads: the program must terminate and no line may be mixed |
 
### What to verify in the output
 
- Every `is compiling` line is preceded by two `has taken a dongle` lines for the same coder.
- Timestamps never go backwards, and no line is printed after `burned out`.
- The program always terminates (no hang) and does not crash.

## Resources
 
### References
 
- `man 3 pthread_create`, `pthread_mutex_lock`, `pthread_cond_wait`, `pthread_cond_timedwait`, `pthread_cond_broadcast`, `gettimeofday`, `usleep`
- https://www.geeksforgeeks.org/c/multithreading-in-c/
- https://youtu.be/VSkvwzqo-Pk
- https://youtu.be/xoXzp4B8aQk
- 
### Use of AI
 
AI was used as a learning and verification tool. The source code of the project was designed and written by me. AI was used for:
 
- **Explaining concepts**: threads, mutexes, condition variables, `pthread_cond_timedwait`, Coffman's conditions and how to break them, and the difference between FIFO and EDF scheduling.
- **Global guidance** during the project: discussing the architecture and the order of the steps.
- **Debugging and code review**: reviewing parts of the code, thinking through race conditions, lock ordering and shutdown paths. Every suggestion was checked and tested before being kept.
- **Tests**: brainstorming test scenarios (invalid arguments, burnout cases, cooldown, many threads) and the Valgrind / Helgrind / ThreadSanitizer checks.
- **Documentation**: help with structuring and writing this `README.md`, which I then reviewed against my code.
 