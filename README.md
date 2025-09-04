# Topological_sort

Finds **all possible topological orderings** of a directed acyclic graph (DAG) using a backtracking algorithm in C.

## Features

- **Cycle Detection:** Verifies input is a DAG using DFS and a three-color system.
- **Backtracking:** Recursively generates every valid topological ordering.
- **Constraint Checking:** Ensures all predecessors of each vertex are placed before it.
- **Solution Generation:** Outputs all orderings, not just one.

## Usage

1. **Clone the repo:**
   ```sh
   git clone https://github.com/HoomanMoradnia/Topological_sort.git
   cd Topological_sort
   ```

2. **Compile:**
   ```sh
   gcc -o topo_sort main.c
   ```

3. **Run:**  
   Prepare an input file or modify `main.c` as needed to define your graph.
   ```sh
   ./topo_sort
   ```

## Example

Given a graph:
- A → B
- A → C
- B → D
- C → D

Possible topological orderings output:
```
A B C D
A C B D
```

## License

MIT License

---

**Author:** [HoomanMoradnia](https://github.com/HoomanMoradnia)
