# Control Flow Graph Analysis

A C++ implementation of data flow analysis algorithms for control flow graphs, focused on reaching definitions analysis. This project demonstrates the convergence properties of iterative data flow algorithms on randomly generated and predefined control flow graphs.

## Features

- **Control Flow Graph Construction**: Build directed graphs representing program control flow
- **Reaching Definitions Analysis**: Implemented iterative algorithm with convergence tracking
- **Realistic Graph Generation**: Generate control flow graphs with meaningful def/kill relationships
- **BitVector Operations**: Efficient bit manipulation for data flow sets
- **Convergence Testing**: Analyze algorithm performance on various graph structures

## Project Structure

```t
Graphs/
├── inc/
│   ├── BasicBlock.h        # Basic block representation
│   ├── BitVector.h         # BitVector operations (OR, AND, subtract)
│   └── ControlFlowGraph.h  # Main CFG class with analysis algorithms
├── src/
│   └── main.cc            # Test cases and graph generation
├── CMakeLists.txt         # Build configuration
└── README.md             # This file
```

## Building and Running

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+)
- CMake 3.6+

### Build Instructions

```bash
mkdir build && cd build

cmake ..

make

./Graph
```

## Usage Examples

The program runs several test cases:

1. **Predefined Small Graph**: A manually constructed 3-block CFG
2. **Generated Graphs**: Randomly generated CFGs with different sizes

## Algorithm Implementation

### Reaching Definitions

The reaching definitions algorithm determines which definitions of variables reach each program point. It uses the following iterative approach:

```cpp
auto Changed = true;
while (Changed) {
    Changed = false;
    for (auto BB : all_blocks) {
        auto NR = BitVector{N, 0}; // new IN vector
        for (auto Pred : preds(BB))
            NR = NR or ROUT[Pred];
        auto RO = GEN[BB] or (NR subtract KILL[BB]) // new out vector
        if (RIN[BB] != NR)
            tie(RIN[BB], Changed) = (NR, true);
        if (ROUT[BB] != RO)
            tie(ROUT[BB], Changed) = (RO, true);
    }
}
```

**Key Components:**

- **GEN[BB]**: Definitions generated in basic block BB
- **KILL[BB]**: Definitions killed (overwritten) in basic block BB
- **RIN[BB]**: Definitions reaching the entry of BB
- **ROUT[BB]**: Definitions reaching the exit of BB

### Graph Generation Strategy

The `GenerateAndTest` function creates realistic control flow graphs:

1. **Linear Backbone**: Ensures graph connectivity
2. **Random Edges**: Adds loops and branches for complexity
3. **Realistic Def/Kill**: Variables are killed when redefined
4. **Convergence Analysis**: Tracks iterations and warns on poor convergence

## Data Structures

### BitVector

- Efficient boolean vector operations
- Supports OR (`|`), AND (`&`), NOT (`~`), and subtract operations
- Used for representing def/kill sets

### BasicBlock

- Represents a basic block in the control flow graph
- Contains predecessor/successor lists
- Stores definition and kill sets

### ControlFlowGraph

- Main container for the entire control flow graph
- Manages bit vectors for data flow analysis
- Implements the reaching definitions algorithm

## Testing and Validation

### Convergence Properties

- Monitors iteration count for convergence analysis
- Warns when convergence is slower than expected (> 2×graph_size)
- Tests graphs of various sizes and complexities

### Graph Validation

- Ensures generated graphs are connected
- Verifies realistic def/kill relationships
- Tests edge cases and boundary conditions

## Future Enhancements

The framework is designed to be extensible for other data flow analyses:

### Available Expressions

```cpp
auto Changed = true;
while (Changed) {
    Changed = false;
    for (auto BB : all_blocks) {
        auto NAVIN = BitVector{N, 0}; // new AVIN vector
        for (auto Pred : preds(BB))
            NAVIN = NAVIN and AVOUT[Pred];
        auto NAVOUT = GEN[BB] or (NAVIN subtract KILL[BB]) // new AVOUT vector
        if (AVIN[BB] != NAVIN)
            tie(AVIN[BB], Changed) = (NAVIN, true);
        if (AVOUT[BB] != NAVOUT)
            tie(AVOUT[BB], Changed) = (NAVOUT, true);
    }
}
```

### Live Variables

```cpp
auto Changed = true;
while (Changed) {
    Changed = false;
    for (auto BB : all_blocks) {
        auto NLVOUT = BitVector{N, 0}; // new LVOUT vector
        for (auto Succ : succ(BB))
            NLVOUT = NLVOUT and LVIN[Succ];
        auto NLVIN = GEN[BB] or (NLVOUT subtract KILL[BB]) // new LVIN vector
        if (LVIN[BB] != NLVIN)
            tie(LVIN[BB], Changed) = (NLVIN, true);
        if (LVOUT[BB] != NLVOUT)
            tie(LVOUT[BB], Changed) = (NLVOUT, true);
    }
}
```

## Performance Characteristics

- **Time Complexity**: O(N × E × V) where N is iterations, E is edges, V is variables
- **Space Complexity**: O(B × V) where B is basic blocks, V is variables
- **Convergence**: Typically O(depth) iterations for acyclic graphs

## Contributing

Contributions are welcome! Areas for improvement:

- Additional data flow analyses
- Graph visualization
- Performance optimizations
- More sophisticated graph generation

## License

This project is for educational purposes, demonstrating data flow analysis concepts.
