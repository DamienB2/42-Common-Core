*This project has been created as part of the 42 curriculum by dbaise.*
 
# get_next_line
 
## Description
 
`get_next_line` is a C function that reads one line at a time from a file descriptor, returning it on each successive call. The goal of the project is to implement a function with the following prototype:
 
```c
char *get_next_line(int fd);
```
 
Each call returns the next line (including the trailing `\n` if present) from the given file descriptor. It returns `NULL` when the end of file is reached or when an error occurs. The function works both with regular files and with standard input (`stdin`).
 
The project introduces an essential concept in C programming: **static variables**, which allow a function to retain state between calls without relying on global variables.
 
---
 
## Algorithm
 
### Approach: Static Stash Buffer
 
The implementation uses a single static `char` pointer called `stash` that persists between calls to `get_next_line`. This is the core mechanism that makes repeated line-by-line reading possible without re-reading from the beginning on each call.
 
The algorithm works in three phases:
 
#### 1. Read and accumulate — `read_file_and_stash`
 
The function reads from the file descriptor in chunks of `BUFFER_SIZE` bytes, appending each chunk to the stash via `ft_strjoin`. Reading continues until a newline character (`\n`) is found in the stash or `EOF` is reached. This ensures the stash always contains at least one complete line before extraction.
 
#### 2. Extract the line — `create_line`
 
Once the stash contains enough data, the next line is extracted up to and including the `\n` using `ft_substr`. The remainder of the stash (everything after the `\n`) is saved back into the static pointer so the next call can pick up where it left off. If no `\n` is present (last line without newline), the entire stash content is returned as the line.
 
#### 3. Return and clean up
 
The extracted line is returned to the caller. If the stash is empty after extraction (EOF reached with no remaining content), it is freed and set to `NULL`, and the function returns `NULL`.
 
### Why this approach?
 
- **Static variable:** A single static pointer retains state between calls with no need for an external data structure or caller-managed state. Only one static variable is used across the entire implementation.
- **Chunk-based reading:** Reading in `BUFFER_SIZE` chunks is efficient regardless of line length — it avoids reading one byte at a time (slow) while still supporting arbitrarily long lines and any buffer size value.
- **Memory safety:** Every intermediate allocation is freed before being replaced. Error paths (failed `malloc`, failed `read`) free all allocated memory and return `NULL`, preventing leaks.
---
 
## Instructions
 
### Compilation
 
Compile with the standard flags. The `BUFFER_SIZE` can be set at compile time with `-D`:
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```
 
The project also compiles without the flag (defaults to `BUFFER_SIZE 10`):
 
```bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c -o gnl
```
 
### Execution
 
To test with a file, uncomment the `main` function in `get_next_line.c` and compile:
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl && ./gnl
```
 
### Usage in your own project
 
Include the header and call the function in a loop:
 
```c
#include "get_next_line.h"
 
int fd = open("myfile.txt", O_RDONLY);
char *line;
 
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```
 
> Always free the returned line after use to avoid memory leaks.
 
---
 
## Resources
 
- [Linux man page — `read(2)`](https://man7.org/linux/man-pages/man2/read.2.html)
- [Linux man page — `open(2)`](https://man7.org/linux/man-pages/man2/open.2.html)
- [Static variables in C — GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [42 Docs — get_next_line overview (GitBook)](https://harm-smits.github.io/42docs/projects/get_next_line)
### AI Usage
 
AI (Claude) was used in the following ways during this project:
 
- **README generation:** The full content of this README was written with the help of Claude, based on the existing implementation and the project subject.
- **Debugging memory leaks:** When valgrind reported leaks, Claude helped identify which allocations were not being freed correctly — specifically in the error path of `read_file_and_stash` when `read()` returns -1, and in the handling of the stash after `create_line` sets it to `NULL`.
The core implementation logic (algorithm design, function structure, static variable usage) was developed independently before consulting AI.
