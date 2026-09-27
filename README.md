# 🌪️ VORTEX (Vector Optimized Retrieval & Text Extraction eXecutable)

**VORTEX** is a high-performance, in-memory text search engine engineered entirely in modern C++. It is designed to act as a lightweight retrieval backend for local environments, operating efficiently even on hardware constrained to standard 8GB memory limits.

By avoiding heavy external dependencies, VORTEX serves as a foundational architecture for offline RAG (Retrieval-Augmented Generation) pipelines, prioritizing cache locality, manual memory management, and algorithmic efficiency.

---

![C++](https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white) ![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge) ![Status](https://img.shields.io/badge/Status-Active_Development-success.svg)

## 🧠 System Architecture & Core Algorithms

VORTEX is built on top of carefully selected data structures and algorithms to ensure sub-millisecond retrieval times, advanced language processing, and high accuracy:

* **Inverted Index (Core Retrieval):** Utilizes `std::unordered_map` mapping terms to document IDs and frequencies. Ensures O(1) average time complexity for direct token lookups without full-table scans.
* **Prefix Trie (Autocomplete):** Implements an n-ary tree structure for real-time search suggestions. Provides O(L) time complexity (where L is word length) for prefix matching, independent of the total dataset size.
* **Vector Mathematics (Ranking):** Implements **TF-IDF with Smoothing** to prevent ubiquitous terms from skewing mathematical weights, paired with **Cosine Similarity** to accurately rank multi-word phrases.
* **Levenshtein Distance (Typo Tolerance):** Utilizes dynamic programming to calculate character edit distances, enabling robust "fuzzy search" that catches misspellings (e.g., mapping "algotihm" to "algorithm").
* **Linguistic Processing (Optimization):** Centralized text normalization strips punctuation and standardizes cases, while a hardcoded **Stop-Word Filter** aggressively blocks grammatical glue words ("the", "is", "at") from consuming index memory.
* **Persistent Storage (Serialization):** Features a custom File I/O pipeline that serializes the complex C++ Inverted Index into a lightweight text file (`vortex_index.txt`), allowing the engine to instantly load its "brain" into RAM on startup without rebuilding from raw data.

---

## 📂 Repository Structure

The project strictly follows enterprise C++ layout conventions, separating declarations, implementations, and build artifacts.

```text
vortex/
├── include/              # Header files (Class blueprints and utilities)
│   ├── inverted_index.hpp
│   ├── trie.hpp
│   └── utils.hpp         # Text normalizer, stop-word filter, & fuzzy match logic
├── src/                  # Source files (Core logic and implementations)
│   ├── inverted_index.cpp
│   ├── trie.cpp
│   └── main.cpp          # Entry point containing the interactive CLI loop
├── data/                 # Raw unstructured text files for indexing
├── CMakeLists.txt        # CMake build configuration
├── .gitignore            # Excludes build binaries and environment cache
└── README.md             # Project documentation

```

---

## 🚀 Getting Started

### Prerequisites

* **C++ Compiler:** MSVC (Visual Studio Build Tools 2022) or GCC/Clang with C++17 support.
* **Build System:** CMake (Version 3.10 or higher).

### Build Instructions

VORTEX uses CMake for cross-platform build generation. To compile the engine from source:

```bash
# 1. Clone the repository
git clone https://github.com/s7d4007/vortex.git
cd vortex

# 2. Generate build files (configured for MSVC/Windows)
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64

# 3. Build the executable
cmake --build . --config Debug

# 4. Run the engine (Launches the interactive CLI)
.\Debug\vortex.exe

```

---

## 🗺️ Development Roadmap

* [X] **Phase 1:** Core Inverted Index structure and memory-safe posting lists.
* [X] **Phase 2:** Trie data structure for low-latency autocomplete suggestions.
* [X] **Phase 3:** TF-IDF smoothed mathematical scoring and Cosine Similarity multi-word ranking.
* [X] **Phase 4:** Linguistic Processing (Stop-word removal, lowercase normalization, punctuation stripping).
* [X] **Phase 5:** Fuzzy Matching (Levenshtein Distance) and interactive CLI loop integration.
* [X] **Phase 6:** Persistent Disk Storage via File I/O serialization.
* [ ] **Phase 7:** Local Filesystem Crawler (Ingesting physical `.txt`, `.md`, and `.cpp` files from the hard drive).
* [ ] **Phase 8:** Decoupled REST API layer for integration with frontend interfaces or local LLMs.

---

## 👨‍💻 Author

**Souvik Shomenath Dutta**

* **GitHub:** [@s7d4007](https://github.com/s7d4007?utm_source=gemini)
* **LinkedIn:** [Souvik Dutta](https://linkedin.com/in/souvikdutta7?utm_source=gemini)

> "Optimizing systems from the hardware up."