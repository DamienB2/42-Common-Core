*This project has been created as part of the 42 curriculum by dbaise.*

# ft_printf

## Description

`ft_printf` is a partial reimplementation of the C standard library function `printf`. The goal is to understand how variadic functions work and how formatted output is built character by character, without relying on any external library.

The function handles the following conversions:

| Specifier | Description |
|-----------|-------------|
| `%c` | Single character |
| `%s` | String (prints `(null)` if pointer is NULL) |
| `%d` / `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal integer (lowercase) |
| `%X` | Unsigned hexadecimal integer (uppercase) |
| `%p` | Pointer address in hex, prefixed with `0x` (prints `(nil)` if NULL) |
| `%%` | Literal percent sign |

The function returns the total number of characters printed, matching the behaviour of the real `printf`.

## Instructions

### Compilation

```bash
make
```

This produces `libftprintf.a`, a static library you can link against your own programs.

To clean object files:
```bash
make clean
```

To remove everything including the library:
```bash
make fclean
```

To recompile from scratch:
```bash
make re
```

### Usage

Link `libftprintf.a` when compiling your program:

```bash
gcc -Wall -Wextra -Werror -I. main.c libftprintf.a -o my_program
./my_program
```

Example `main.c`:

```c
#include "ft_printf.h"

int main(void)
{
    ft_printf("Hello, %s!\n", "world");
    ft_printf("Dec: %d | Hex: %x | Ptr: %p\n", 42, 255, (void *)0x1234);
    return (0);
}
```

## Algorithm and Data Structure

### Parsing loop — `ft_printf.c`

`ft_printf` walks the format string one character at a time using a pointer `pos`. When it encounters a `%`, it advances to the next character and calls the static helper `process()`, which reads the specifier and dispatches to the appropriate output function via `va_arg`. For any other character it simply writes it directly to stdout with `ft_putchar`. A running counter tracks the total number of characters printed and is returned at the end.

This approach avoids building any intermediate buffer: every character is written immediately via `write(1, ...)`, which keeps memory usage constant regardless of the format string length.

### Output helpers — `utils.c`

- `ft_putchar` — writes a single byte with `write`.
- `ft_putstr` — counts the string length with a `while` loop, then writes the whole string in one `write` call. Returns 6 on a NULL pointer (`"(null)"`).
- `ft_putnbr` — handles `INT_MIN` as a special case (to avoid undefined behaviour when negating it), then uses recursion to print digits from most to least significant.
- `ft_unsigned_putnbr` — same recursive approach without the sign handling.
- `ft_putaddr` — prefixes the address with `"0x"` and delegates the hex conversion to `convert()`. Prints `"(nil)"` for NULL.

### Base conversion — `convert.c`

`convert` uses a **static character buffer of 50 bytes** filled from right to left. Starting from the last position, it repeatedly takes `num % base` to get the least significant digit and divides `num` by `base`, writing each digit into the buffer. It then returns a pointer into that buffer starting at the first significant digit. This avoids any dynamic allocation and works for both base 10 and base 16, upper or lowercase, depending on the `isUpper` flag.

The choice of a static buffer is a deliberate trade-off: it is simple and allocation-free, but means the function is not reentrant. For this project's scope (single-threaded formatting) it is perfectly appropriate.

## Resources

- [printf(3) — Linux man page](https://man7.org/linux/man-pages/man3/printf.3.html)
- [Variadic functions in C — GeeksforGeeks](https://www.geeksforgeeks.org/c/variadic-functions-in-c/)
- [ft_printf guide — 42 Cursus Gitbook](https://42-cursus.gitbook.io/guide/1-rank-01/ft_printf)

### Use of AI

Claude (Anthropic) was used during this project for the following tasks:
- Debugging compilation errors when removing the `libft` dependency (identifying that `ft_putchar_fd` needed to be replaced by `ft_putchar`, and that `libft.h` includes needed to be replaced by standard headers).
- Generating this README based on the actual source code.

The core logic — the parsing loop, output functions, and base conversion — was written entirely by the student.