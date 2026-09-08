# 🌪️ VORTEX (Vector Optimized Retrieval & Text Extraction eXecutable)

![C++](https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active_Development-success.svg)

**VORTEX** is a high-performance, in-memory text search engine engineered entirely in modern C++. It is designed to act as a lightweight retrieval backend for local environments, operating efficiently even on hardware constrained to standard 8GB memory limits.

By avoiding heavy external dependencies, VORTEX serves as a foundational architecture for offline RAG (Retrieval-Augmented Generation) pipelines, prioritizing cache locality, manual memory management, and algorithmic efficiency.

---

## 🧠 System Architecture & Core Algorithms

VORTEX is built on top of carefully selected data structures to ensure sub-millisecond retrieval times across thousands of documents:

* **Inverted Index (Core Retrieval):** Utilizes std::unordered_map mapping terms to document IDs and frequencies. Ensures O(1) average time complexity for direct token lookups without full-table scans.
* **Prefix Trie (Autocomplete):** Implements an n-ary tree structure for search suggestions. Provides O(L) time complexity (where L is word length) for prefix matching, completely independent of the total dataset size.
* **Min-Heap Priority Queues (Ranking):** Maintains a Top-K relevance ranking using TF-IDF (Term Frequency-Inverse Document Frequency) scoring, retrieving the most contextually relevant documents in O(N log K) time.

---

## 📂 Repository Structure

The project strictly follows enterprise C++ layout conventions, separating declarations, implementations, and build artifacts.

vortex/
├── include/              # Header files (Class blueprints and data structures)
│   ├── inverted_index.hpp
│   └── trie.hpp
├── src/                  # Source files (Core logic and implementations)
│   ├── inverted_index.cpp
│   ├── trie.cpp
│   └── main.cpp
├── data/                 # Raw unstructured text files for indexing
├── CMakeLists.txt        # CMake build configuration
├── .gitignore            # Excludes build binaries and environment cache
└── README.md             # Project documentation

---

## 🚀 Getting Started

### Prerequisites

* **C++ Compiler:** MSVC (Visual Studio Build Tools 2022) or GCC/Clang with C++17 support.
* **Build System:** CMake (Version 3.10 or higher).

### Build Instructions

VORTEX uses CMake for cross-platform build generation. To compile the engine from source:

# 1. Clone the repository

git clone https://github.com/yourusername/vortex.git
cd vortex

# 2. Generate build files (configured for MSVC/Windows)

mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64

# 3. Build the executable

cmake --build . --config Debug

# 4. Run the engine

.\Debug\vortex.exe

---

## 🗺️ Development Roadmap

- [X] Phase 1: Core Inverted Index structure and memory-safe posting lists.
- [ ] Phase 2: Trie data structure for low-latency autocomplete suggestions.
- [ ] Phase 3: TF-IDF mathematical scoring and Priority Queue ranking.
- [ ] Phase 4: Decoupled REST API layer for integration with frontend interfaces or local LLMs.

---

## 👨‍💻 Author

**Souvik Shomenath Dutta**

* **GitHub:** [@s7d4007](https://github.com/s7d4007)
* **LinkedIn:** [Souvik Dutta](https://linkedin.com/in/souvikdutta7)

> "Optimizing systems from the hardware up."
