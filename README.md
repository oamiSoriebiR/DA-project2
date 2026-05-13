# PLACEHOLDER MADE WITH AI - NEED TO UPDATE WITH REAL CONTENT

---

## DA2026_PRJ2: Compiler Register Allocation

**Course:** Analysis and Synthesis of Algorithms - Design of Algorithms (DA) L.EIC016  
**Academic Year:** Spring 2026  
**Team:** Group 4 - Class ''???''  
**Members:**

- Beatriz Remondes - `[ID]`
- Nuno Coimbra - `[ID]`
- simão Ribeiro - 202306111

---

## Project Overview

This project implements a compiler back-end tool for **global register allocation** using graph-coloring heuristics.

Given live ranges from a 3-address intermediate representation, the tool:

- builds **live webs**,
- constructs an **interference graph**,
- and assigns webs to a limited number of registers.

When allocation is not possible, the project supports **spilling**, **splitting**, and a **hybrid free strategy** to reduce register pressure.

---

## Implemented Features

- **Batch and interactive interface** for testing and manual execution.
- **Input parsing** for live ranges and register configuration.
- **Web construction and merging** from variable live ranges.
- **Interference graph building** using the provided graph structure.
- **Basic register allocation** using a simplify-and-color heuristic.
- **Spilling heuristic** to move selected webs to memory.
- **Splitting heuristic** to divide webs and reduce interference.
- **Free strategy** combining the previous approaches.
- **Documentation** with complexity analysis in the source code.

---

## Build and Run

### Requirements

- **C++17**
- **Make**

### Build

````bash
make
`````

### Execution Modes

#### 1. Batch Mode (Used for grading)

Executes the allocation directly via command-line arguments. Errors/warnings are printed to `stderr`.

```bash
./[executable_name] -b <ranges.txt> <registers.txt> <output_allocation.txt>
```

*Example:*

```bash
./register_alloc -b test_cases/case1_ranges.txt test_cases/case1_regs.txt output.txt
```

#### 2. Interactive Mode

Launches a user-friendly terminal menu to load files, select algorithms, and view results step-by-step.

```bash
./[executable_name]
```

---

## 📂 Input & Output Formats

### Input Files

1. **Ranges File (`ranges.txt`):** Defines variables and their execution live ranges. `+` indicates a definition (write), `-` indicates the last use (read).
2. **Registers File (`registers.txt`):** Defines the maximum number of registers $N$ and the algorithm variant to use.
    - `algorithm: basic`
    - `algorithm: spilling, K` (where $K$ is max webs to spill)
    - `algorithm: splitting, K` (where $K$ is max webs to split)
    - `algorithm: free`

### Output File (`allocation.txt`)

Generates a human-readable map of constructed webs and their assigned registers (`r0`, `r1`...) or memory (`M` if spilled/unable to allocate).

---

## 🧠 Algorithm Design & Heuristics

### Web Spilling Heuristic (T2.2)

Instead of spilling arbitrarily, our algorithm calculates a **Spill Metric** for each uncolorable node: `Degree(Node) / Live_Range_length(Node)`.

- **Rationale:** We want to spill webs that cause the most interference (high degree) but are "cheap" to spill to memory (short live range). This minimizes the memory load/store instructions a compiler would theoretically have to insert. We iterate from $1$ to $K$ spilled webs, stopping as soon as the graph becomes $N$-colorable.

### Web Splitting Heuristic (T2.3)

When a web cannot be colored, we analyze its internal execution points to find the **Maximum Interference Bottleneck**—the specific program line where the web simultaneously interferes with the highest number of other webs.

- **Rationale:** Splitting at this exact point severs the most edges in the interference graph for a single split operation. The original web is divided into `web_part1` (pre-bottleneck) and `web_part2` (post-bottleneck), drastically reducing the graph's chromatic number.

### "Free" Algorithm Approach (T2.4)

Our custom algorithm uses a **Priority-Based Hybrid Strategy**. It modifies the basic simplify phase by using a *Smallest-Degree-First* priority queue instead of a standard stack. If an impasse is reached, it evaluates the graph's overall density:

- If the graph is highly dense (near-clique), it triggers **Spilling**.
- If the graph has distinct localized clusters of high degree, it triggers **Splitting**.
This dynamic choice prevents the pitfalls of aggressively splitting highly connected graphs or foolishly spilling easily splittable webs.

---

## 📁 Project Repository Structure

```text
.
├── src/                    # Source code files
│   ├── main.cpp            # Entry point, CLI & Batch parser
│   ├── parser.h/cpp        # Live range & input file parsing
│   ├── web.h/cpp           # Web data structure & merging logic
│   ├── graph.h/cpp         # Interference graph (Based on class template)
│   ├── algorithms.h/cpp    # Basic, Spilling, Splitting, Free logic
│   └── io.h/cpp            # Output file generation
├── output/                 # Output files
├── docs/                   # Generated Doxygen HTML documentation
├── presentation.pdf        # Demo presentation slides
├── data/                   # Input files
├── README.md               # This file
└── Makefile                # Build script
```

---

## ⏱️ Time Complexity Analysis

- **Web Merging:** $O(V \cdot L^2)$ where $V$ is the number of variables and $L$ is the max number of ranges per variable.
- **Interference Graph Construction:** $O(W^2 \cdot P)$ where $W$ is the number of merged webs and $P$ is the average number of program points per web.
- **Basic Simplify/Color Phase:** $O(W^2)$ using an adjacency matrix / $O(W + E)$ using adjacency lists, executed iteratively.
- **Spilling/Splitting Phases:** Multiplicative factor of $O(K)$ on top of the Basic phase, where $K$ is the allowed spill/split limit.

*(See the `doc/html/` folder for detailed function-by-function Doxygen complexity notes).*
