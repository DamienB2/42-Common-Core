*This project has been created as part of the 42 curriculum by dbaise, dmonseur.*
 
# push_swap
 
## Description
 
**push_swap** is a sorting algorithm project from the 42 curriculum. The goal is to sort a list of integers stored in a stack, using only a restricted set of operations on two stacks (`a` and `b`), while minimising the total number of operations produced.
 
The program reads a list of integers (stack `a`), computes a **disorder metric** to assess how unsorted the input is, selects the most appropriate sorting strategy, and prints the resulting sequence of operations to stdout.
 
Four distinct strategies are embedded in the binary: a simple O(n²) algorithm, a medium O(n√n) algorithm, a complex O(n log n) algorithm, and a custom adaptive algorithm that picks the best strategy at runtime based on the measured disorder.
 
Available operations: `sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`.
 
---
 
## Instructions
 
### Compilation
 
```bash
make        # builds push_swap
make clean  # removes object files
make fclean # removes object files and the binary
make re     # full rebuild
```
 
> `libft` is compiled automatically as a dependency.
 
### Usage
 
```bash
./push_swap [--simple|--medium|--complex|--adaptive] [--bench] <integers...>
```
 
| Flag | Description |
|---|---|
| *(none)* | Adaptive mode (default) |
| `--simple` | Forces the O(n²) insertion sort |
| `--medium` | Forces the O(n√n) chunk sort |
| `--complex` | Forces the O(n log n) radix sort |
| `--adaptive` | Selects strategy based on computed disorder |
| `--bench` | Prints benchmark metrics to stderr after sorting |
 
### Examples
 
```bash
# Sort a small list (adaptive, default)
./push_swap 5 3 1 4 2
 
# Force a specific strategy
./push_swap --simple 5 4 3 2 1
./push_swap --complex 42 7 -3 100 0
 
# Count operations
ARG="4 67 3 87 23"; ./push_swap --adaptive $ARG | wc -l
 
# Verify correctness with a checker
ARG="4 67 3 87 23"; ./push_swap --complex $ARG | ./checker_linux $ARG
 
# Run with full benchmark output (sent to stderr)
./push_swap --bench 5 3 1 4 2 2>/dev/null   # hide metrics, keep ops
./push_swap --bench 5 3 1 4 2 1>/dev/null   # hide ops, keep metrics
```
 
### Error handling
 
```bash
./push_swap 0 one 2 3      # Error (non-integer argument)
./push_swap 3 2 3          # Error (duplicate value)
./push_swap 9999999999 1   # Error (integer overflow)
```
 
---
 
## Algorithms
 
### Disorder metric
 
Before any sorting begins, the program computes a **disorder score** in [0, 1] using the number of *inversions* (pairs where a larger value appears before a smaller one) divided by the total number of pairs:
 
```
disorder = inversions / (n * (n - 1) / 2)
```
 
A score of `0` means the stack is already sorted; `1` means it is in reverse order.
 
---
 
### 1. Insertion Sort — O(n²) `--simple`
 
**File:** `insertion_sort.c`
 
This is a selection-sort-style adaptation for stacks. For each rank `i` from `0` to `n-1`:
 
1. Determine whether rotating up (`ra`) or rotating down (`rra`) reaches element of rank `i` faster, using `find_direction`.
2. Bring that element to the top of stack `a`.
3. Push it to stack `b`.
Once all elements are in `b`, they are pushed back to `a` in reverse order (largest first), using the same rotation logic.

**Adaptive threshold:** used when `disorder < 0.2`.

 
---
 
### 2. Chunk Sort — O(n√n) `--medium`
 
**File:** `chunk_sort.c`
 
The input is divided into **√n chunks** of equal size based on rank ranges:
 
1. Push all elements of chunk `n` from `a` to `b`, rotating `a` to find the next target. Each element costs at most O(√n) rotations to locate.
2. Once all elements are in `b`, pull them back to `a` using `insertion_sort_reverse`: for each rank from `n-1` down to `0`, locate it in `b` (at most O(n) rotations) and push to `a`.
 
 **Adaptive threshold:** used when `0.2 ≤ disorder < 0.5`.

---
 
### 3. Radix Sort LSD — O(n log n) `--complex`
 
**File:** `radix_sort.c`
 
After indexing all values to ranks `[0, n)`, the algorithm processes each bit of the binary representation of those ranks, from the least significant bit (LSD) to the most significant:
 
1. For each bit position, scan all elements currently in `a`:
   - If the current bit is `0`, push the element to `b`.
   - If the current bit is `1`, rotate it to the bottom of `a` (`ra`).
2. Push all elements back from `b` to `a` (`pa`).
3. Repeat for the next bit.
After `⌈log₂ n⌉` passes the elements are sorted by rank, hence by value.
 
**Adaptive threshold:** used when `disorder ≥ 0.5`.
 
---
 
### 4. Adaptive Strategy `--adaptive` (default)
 
**File:** `strat_selector.c`
 
This strategy selects one of the three algorithms above based on the disorder score measured before any moves:
 
| Disorder range | Selected algorithm | Complexity |
|---|---|---|
| `disorder < 0.2` | Insertion Sort | O(n²) |
| `0.2 ≤ disorder < 0.5` | Chunk Sort | O(n√n) |
| `disorder ≥ 0.5` | Radix Sort LSD | O(n log n) |
 
**Rationale for thresholds:**
 
- Below `0.2`, the stack is nearly sorted. Very few inversions exist, so the O(n²) insertion sort's small constant makes it competitive, and it takes advantage of existing order naturally.
- Between `0.2` and `0.5`, the stack has moderate disorder. Chunk sort amortises the cost by grouping elements into ranges, outperforming the simple sort without needing the full overhead of a bitwise pass.
- At or above `0.5`, the stack is highly disordered. Radix sort's consistent O(n log n) behaviour regardless of input order gives the best asymptotic guarantee.
---

 
## Performance Targets for --complex
 
| Input size | Minimum | Good | Excellent |
|---|---|---|---|
| 100 numbers | < 2 000 ops | < 1 500 ops | < 700 ops |
| 500 numbers | < 12 000 ops | < 8 000 ops | < 5 500 ops |
 
---
 
## Resources

- [GeeksForGeeks — Insertion Sort](https://www.geeksforgeeks.org/dsa/insertion-sort-algorithm/)
- [GeeksForGeeks — Radix Sort](https://www.geeksforgeeks.org/dsa/radix-sort/)
- [Satyadeep Maheshwari — Sorting Large Datasets with Limited Memory (Chunked approach)](https://satyadeepmaheshwari.medium.com/sorting-large-datasets-with-limited-memory-the-chunked-merge-sort-approach-318275275c81)
- [push_swap visualizer](https://push-swap42-visualizer.vercel.app/) — interactive tool used during development to visually verify operation sequences.

### AI usage

AI was used during this project for the following tasks:

- **Conceptual explanations:** AI was mainly used to better understand the principles and inner workings of each sorting algorithm step by step (insertion sort, chunk sort, radix sort LSD), without generating any direct code. This helped us build a clearer mental model before writing the implementation ourselves.
- **Debugging assistance:** discussing edge cases in argument parsing (empty strings, integer overflow, duplicates) and helping reason about off-by-one errors in rotation direction logic.
- **README drafting:** structuring and writing this document.
