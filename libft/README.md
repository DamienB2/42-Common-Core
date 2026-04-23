*This project has been created as part of the 42 curriculum by dbaise.*
 
# libft
 
## Description
 
**libft** is the first project of the 42 Common Core. The goal is to reimplement a set of standard C library functions from scratch and build a personal static library — `libft.a` — reusable in all future 42 projects.
 
It covers string manipulation, memory management, character checks, file descriptor output, and linked list handling through a `t_list` structure.
 
## Description of the library
 
The library compiles into **`libft.a`** and is split into three parts:
 
**Part 1 – Libc reimplementations**
`ft_isalpha` `ft_isdigit` `ft_isalnum` `ft_isascii` `ft_isprint` `ft_toupper` `ft_tolower` `ft_strlen` `ft_strchr` `ft_strrchr` `ft_strncmp` `ft_strnstr` `ft_strlcpy` `ft_strlcat` `ft_strdup` `ft_atoi` `ft_memset` `ft_bzero` `ft_memcpy` `ft_memmove` `ft_memchr` `ft_memcmp` `ft_calloc`
 
**Part 2 – Additional utilities**
`ft_substr` `ft_strjoin` `ft_strtrim` `ft_split` `ft_itoa` `ft_strmapi` `ft_striteri` `ft_putchar_fd` `ft_putstr_fd` `ft_putendl_fd` `ft_putnbr_fd`
 
**Part 3 – Linked list**
`ft_lstnew` `ft_lstadd_front` `ft_lstadd_back` `ft_lstlast` `ft_lstsize` `ft_lstdelone` `ft_lstclear` `ft_lstiter` `ft_lstmap`
 
## Instructions
 
```bash
make        # compile the library
```
 
## Resources
 
- Man pages
- Peer-to-peer learning
- YouTube videos
- AI (Claude) — used after writing each function to review correctness and check edge cases, never to generate code directly
