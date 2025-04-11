# Triangle Lattice Project

This project provides tools and utilities for working with triangle lattice structures, including operations on bond and site graphs, topology generation, and graph manipulation.

## Features

- **Bond Triangle Utilities**: Functions for creating, manipulating, and analyzing bond graphs in triangular lattices.
- **Site Triangle Utilities**: Tools for handling site-based graphs and their corresponding topologies.
- **Topology Generation**: Algorithms to generate and analyze topologies based on bond and site graphs.
- **Graph Embedding**: Utilities for embedding graphs into lattice structures.
- **Subgraph Analysis**: Functions for extracting and analyzing subgraphs from larger lattice structures.

## Project Structure

```
TriangleLattice/
├── bondtriangle/
│   ├── convertformarcos/       # Utilities for converting and analyzing bond graphs
│   ├── squareutils.c           # Core utilities for square lattice operations
│   ├── topoutils.c             # Functions for topology generation and manipulation
│   └── maketopolist.c          # Script for generating topology lists
├── sitetriangle/
│   ├── sitesquareutils.c       # Utilities for site-based graph operations
│   ├── maketrlist.c            # Script for creating triangle lattice lists
│   └── makesitesubtopolist.c   # Functions for subgraph extraction and analysis
└── .nstrumenta/
    ├── credentials/            # Directory for storing credentials
    └── readme.md               # Instructions for setting up credentials
```

## Getting Started

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd linked-cluster
   ```

2. Set up credentials:
   - Add a `.env` file in `.nstrumenta/credentials/` with the required environment variables.

3. Build and run the project:
   - Use the provided `Dockerfile` to create a containerized environment.
   - Compile the C files using `gcc` or your preferred compiler.

## Key Functionalities

### Bond Triangle Utilities
- **`squareutils.c`**: Provides functions for comparing bonds, rotating graphs, and managing adjacency matrices.
- **`topoutils.c`**: Handles topology generation, embedding, and adjacency matrix manipulation.

### Site Triangle Utilities
- **`sitesquareutils.c`**: Includes functions for site graph manipulation, bond creation, and subgraph extraction.
- **`maketrlist.c`**: Generates lists of triangle lattice structures.