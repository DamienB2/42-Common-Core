# get_next_line

## Description

get_next_line is a C function that reads a single line at a time from a file descriptor, returning it on each successive call. The goal of the project is to implement a function with the following prototype:

```c
char *get_next_line(int fd);
```

Each call returns the next line (including the trailing `\n` if present) from the given file descriptor. It returns `NULL` when the end of file is reached or when an error occurs.

---

## Algorithm

### Approach: Static Stash Buffer

The implementation uses a static char pointer (`stash`) that persists between calls to `get_next_line`. This is the core mechanism that makes repeated line-by-line reading possible without re-reading from the beginning each time.

The algorithm works in three phases:

#### 1. Read and accumulate (`read_file_and_stash`)

The function reads from the file descriptor in chunks of `BUFFER_SIZE` bytes, appending each chunk to the stash via `ft_strjoin`. Reading continues until a newline character (`\n`) is found in the stash, or `EOF` is reached. This ensures the stash always contains at least one complete line.

#### 2. Extract the line (`create_line`)

Once the stash contains enough data, the next line is extracted up to and including the `\n`. The remainder of the stash (everything after the `\n`) is saved back into the static stash pointer so the next call can pick up where it left off.

#### 3. Return and clean up

The extracted line is returned to the caller. On EOF with no remaining content, the stash is freed and set to `NULL`, and the function returns `NULL`.

### Why this approach?

- **Static variable:** A single static pointer retains state between calls with no need for an external data structure or caller-managed state.
- **Chunk-based reading:** Reading in `BUFFER_SIZE` chunks is efficient regardless of line length — it avoids reading one byte at a time (slow) while still supporting arbitrarily long lines.
- **Memory safety:** Every intermediate allocation is freed before being replaced, and error paths (failed `malloc`, failed `read`) free all allocated memory and return `NULL`.

---

## Instructions

### Compilation

Compile the project with your own source files using `gcc`. The `BUFFER_SIZE` can be set at compile time:

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```

To use the included main (for testing):

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl && ./gnl
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

> Always free the returned line after use.

---

## Resources

### Documentation & References

- POSIX read man page
- W3School
- GitBook on get_next_line

## AI Usage


- **Generating this README**
- **Helping me solve leak problems**