*This project has been created as part of the 42 curriculum by dbaise.*
 
# ft_printf
 
## Description
 
`ft_printf` is a partial reimplementation of the C standard library function `printf()`. The goal is to understand how variadic functions work in C, and to handle the formatting and output of several conversion specifiers — without relying on the original `printf()` or its internal buffer management.
 
Supported specifiers: `%c`, `%s`, `%d`, `%i`, `%u`, `%x`, `%X`, `%p`, `%%`.
 
The function returns the total number of characters written.
 
---
 
## Instructions
 
### Prerequisites
 
This project depends on [libft](https://github.com/your-repo/libft). Make sure it is present at `../libft/` relative to the `ft_printf` directory, and already compiled (`libft.a`).
 
### Compilation
 
```bash
make
```
 
This produces `libftprintf.a`, the static library to link against.
 
### Cleaning
 
```bash
make clean    # removes object files
make fclean   # removes object files + library
make re       # fclean + full rebuild
```
 
---
 
## Resources
 
### Documentation
 
- Man pages
- Websites like GeeksforGeeks, StackOverflow, W3Schools, etc
- AI — used to create a main to review correctness and check edge cases, never to generate code directly
 
