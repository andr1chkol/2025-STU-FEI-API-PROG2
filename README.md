# Programming 2 Coursework

C coursework projects from the Programming 2 course at STU FEI.

The repository contains several practical assignments focused on structured C programming, file input/output, string processing, arrays, pointers, dynamic memory, basic parsing, and simple simulation logic.

## Projects

### PS_1 - Activation Function Tables

Generates formatted tables for several mathematical activation functions. The program reads a numeric range, formatting options, and optional function IDs from standard input.

Main topics:

- math functions
- formatted output
- input validation
- table generation

### PS_2 - Neural Network Digit Classification

Implements selected parts of a small feed-forward neural network for digit recognition. The assignment works with 28x28 image data, model weights, bias values, neuron outputs, softmax, and prediction accuracy.

Main topics:

- arrays and matrix-like indexing
- image data represented as numeric input
- weighted sums and bias values
- softmax
- simple classifier evaluation

### PS_3 - String Range Editor

Implements a command-based text range editor. The program keeps an input string, a selected range, and a memory buffer, then processes commands for moving range boundaries, selecting words, copying, cutting, inserting, and replacing text.

Main topics:

- strings and character arrays
- pointers into buffers
- command parsing
- text manipulation

### PS_4 - Battle Simulation

Simulates a turn-based battle between two armies. Units have hit points and equipment, while items define attack, defense, range, radius, and slot usage.

Main topics:

- structs
- arrays of units and items
- dynamic memory
- turn-based simulation
- validation and error handling

### PS_5 - File-Based Army Parser

Extends the battle data model with file input. The program reads an item database from `items.json`, loads armies from text files, validates input, and prints parsed army data.

Main topics:

- file input/output
- simple JSON-like parsing
- TXT army file parsing
- validation errors
- dynamic memory

## Technologies

- C
- CMake for selected assignments
- Bash test scripts
- Standard C library
- `math.h`

## Repository Structure

```text
.
|-- PS_1/       # Activation functions and formatted output
|-- PS_2/       # Neural network digit classification tasks
|-- PS_3/       # Command-based string/range editor
|-- PS_4/       # Turn-based battle simulation
|-- PS_5/       # JSON/TXT parsing for battle data
`-- scripts/    # Shared test runner
```

## Build

Each assignment can be built independently.

Examples:

```bash
gcc PS_1/z1.c -lm -o PS_1/z1
gcc PS_3/z3.c -o PS_3/z3
gcc PS_5/z5.c -o PS_5/z5
```

Assignments with multiple source files can be built with CMake or directly with `gcc`.

```bash
gcc PS_2/src/functions.c PS_2/src/data.c PS_2/src/z2.c -I PS_2/include -lm -o PS_2/z2
gcc PS_4/src/data.c PS_4/src/z4.c -I PS_4/include -lm -o PS_4/z4
```

## Tests

A shared test runner is provided:

```bash
bash scripts/run_tests.sh
```

The default mode compiles the assignments into `.test-build/` and runs a representative smoke test set for each project.

To run the full archived input/output scenarios:

```bash
bash scripts/run_tests.sh --full
```

The full mode is useful for local regression checking. Some historical extra scenarios are kept for reference and may expose differences from the current coursework solutions.

## Test Data

The repository keeps lightweight `stdin` and `stdout` test files used by the runner.

Large generated MNIST conversion folders from local experimentation are intentionally not tracked:

```text
PS_2/z2_testing/mnist-png/
PS_2/z2_testing/mnist-txt/
```

## Notes

- This repository is kept as coursework, not as a production library.
- The code follows the original assignment constraints, so some files are intentionally single-file solutions.
- Assignment PDFs, local IDE files, compiled binaries, and generated build folders are not part of the cleaned repository.
