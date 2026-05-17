## DA2026_PRJ2: Compiler Register Allocation

**Course:** Analysis and Synthesis of Algorithms - Design of Algorithms (DA) L.EIC016  
**Academic Year:** Spring 2026  
**Team:** Group 4 - Class 5  
**Members:**

- Beatriz Remondes - 202204353
- Nuno Coimbra - 202405191
- Simão Ribeiro - 202306111

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
- **Free strategy** combining spilling and splitting dynamically based on a split-efficiency threshold.
- **Documentation** with complexity analysis in the source code.

---

## Build and Run

### Requirements

- **C++17**
- **Make**

### Build

```bash
make
```

### Execution Modes

#### Batch Mode

Executes the allocation directly via command-line arguments. Errors/warnings are printed to `stderr`.

```bash
./allocator -b <ranges.txt> <registers.txt> <output_allocation.txt>
```

#### Interactive Mode

Launches a user-friendly terminal menu to load files, select algorithms, and view results step-by-step.

```bash
./allocator
```

---

## Input & Output Formats

### Input Files

1. **Ranges File (`ranges.txt`):** Defines variables and their execution live ranges. `+` indicates a definition (write), `-` indicates the last use (read).
2. **Registers File (`registers.txt`):** Defines the maximum number of registers $N$ and the algorithm variant to use.<br>
Algorithm parameter must be manually changed to test other algorithms.
    - `algorithm: basic`
    - `algorithm: spilling, K` (where $K$ is max webs to spill)
    - `algorithm: splitting, K` (where $K$ is max webs to split)
    - `algorithm: free`

### Output File

Generates a human-readable map of constructed webs and their assigned registers (`r0`, `r1`...) or memory (`M` if spilled/unable to allocate).<br>

Example:<br>
```text
webs: 3
web0: 1+,2,3-
web1: 2+,3,4,5-
web2: 4+,5,6,7-
registers: 2
r0: web0
r1: web1
r0: web2
```

---

## Algorithm Design & Heuristics

### Web Spilling Heuristic (T2.2)

Instead of spilling arbitrarily, our algorithm calculates a **Spill Metric** for each uncolorable node: `Degree(Node) / Live_Range_length(Node)`.

- **Rationale:** We want to spill webs that cause the most interference (high degree) but are "cheap" to spill to memory (short live range). This minimizes the memory load/store instructions a compiler would theoretically have to insert. We iterate from $1$ to $K$ spilled webs, stopping as soon as the graph becomes $N$-colorable.

### Web Splitting Heuristic (T2.3)

When a web cannot be colored, we analyze its internal execution points to find the **Maximum Interference Bottleneck**—the specific program line where the web simultaneously interferes with the highest number of other webs.

- **Rationale:** Splitting at this exact point severs the most edges in the interference graph for a single split operation. The original web is divided into `web_part1` (pre-bottleneck) and `web_part2` (post-bottleneck), drastically reducing the graph's chromatic number.

### "Free" Algorithm Approach (T2.4)

Our custom algorithm uses a **Dynamic Efficiency Threshold** strategy, actively combining both spilling and splitting based on graph pressure. When an impasse is reached, it evaluates the web with the highest degree:

- **Split Efficiency Metric:** It calculates `Live_Range_length(Node) / Degree(Node)` for the target web.
- **Dynamic Cutoff:** It compares this efficiency against a threshold inversely proportional to the number of available registers ($1.5 / N$).
- **Decision:** If the web's efficiency falls below this cutoff (or if the web spans $\le 2$ lines), splitting would only cause fragment pollution, so the web is **spilled**. Otherwise, it is **split**.
This adaptive choice prevents the pitfalls of over-splitting highly connected but short-lived webs.

---

## Project Repository Structure

```text
├── data/                   # Input files
│   ├── output/             # Output files, 1 for each range file
│   ├── range/              # Variable ranges file
│   └── registers/          # Register & algorithm file
├── src/                    # Source code files
│   ├── main.cpp            # Entry point, CLI & Batch parser
│   ├── parser.h/cpp        # Live range & input file parsing
│   ├── web.h/cpp           # Web data structure & merging logic
│   ├── graph.h             # Graph class (Based on class template)
│   ├── interference.h/cpp  # Interference graph creation
│   ├── algorithms.h/cpp    # Basic, Spilling, Splitting, Free logic
│   ├── io.h/cpp            # Output file generation
│   └── datastructures.h    # Auxiliary data structures (Web, LiveRange, AssignmentConfig)
├── docs/                   # Generated Doxygen HTML documentation
├── README.md               # This file
└── Makefile                # Build script
```

---

## Time Complexity Analysis

- **Web Merging:** $O(R^2 \cdot L)$ where $R$ is the number of ranges and $L$ is the max lines per range.
- **Interference Graph Construction:** $O(W^2 \cdot L)$ where $W$ is the number of webs and $L$ is the max number of lines in a web.
- **Basic Simplify/Color Phase:** $O(V^2 + V \cdot k)$ where $V$ is the number of vertices (webs) and $k$ is the number of registers.
- **Spilling Phase:** $O(V^2 \cdot W + W^2 \cdot L)$ where $V$ is the number of vertices, $W$ is the number of webs, and $L$ is the max number of lines in a web.
- **Splitting & Free Phases:** $O(C \cdot (W^2 \cdot L + V^2))$ where $C$ is a constant max splits, $W$ is the number of webs, $L$ is the max lines per web, and $V$ is the number of vertices.

*(See the `docs/html/` folder for detailed function-by-function Doxygen complexity notes).*
