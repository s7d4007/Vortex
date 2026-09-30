# 🌪️ VORTEX

VORTEX is a lightweight C++ search engine for indexing and searching local documents from a filesystem tree. It is built for offline retrieval workflows, with a recursive file crawler, an inverted index, a prefix trie for suggestions, and cosine-similarity ranking for query results.

The project is intentionally dependency-light and designed to run as a standalone CLI utility without any external database or service layer.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-3.10%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)

---

## What the project does

VORTEX performs the following tasks:

- Recursively crawls a directory and indexes supported text files
- Normalizes and tokenizes document text
- Filters common stop words to reduce noise
- Builds an inverted index for term lookup
- Builds a trie for prefix-driven autocomplete
- Uses TF-IDF-style weighting and cosine similarity to rank search results
- Supports fuzzy matching for near-miss spellings via Levenshtein distance
- Saves and reloads the index and document path map from disk for faster startup

---

## Core architecture

### Inverted index
The engine stores terms as keys and document postings as values. Each posting contains:

- document id
- term frequency

This allows quick retrieval of matching documents for a search term.

### Trie autocomplete
The trie stores normalized words and can return all words that begin with a specified prefix. This powers the suggestion list shown after each query.

### Ranking model
The engine combines inverted-index retrieval with cosine similarity and a TF-IDF-like score to rank documents by relevance.

### Fuzzy search
If a query term is not found exactly, the code compares it against indexed terms using Levenshtein distance and uses the closest match within a configured threshold.

### File cache
When the app is run, it can reuse cached data from:

- `vortex_index.txt`
- `vortex_paths.txt`

These files are generated in the project root and let the search engine reload previously indexed content instead of rebuilding everything from scratch.

---

## Repository layout

```text
Vortex/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── data/
│   └── (empty by default; can hold sample content)
├── include/
│   ├── inverted_index.hpp
│   ├── trie.hpp
│   └── utils.hpp
├── src/
│   ├── inverted_index.cpp
│   ├── trie.cpp
│   └── main.cpp
├── build/
│   └── generated CMake/MSBuild output
├── vortex_index.txt
├── vortex_paths.txt
└── vortex.exe (generated after build)
```

---

## Prerequisites

- C++17 compatible compiler
- CMake 3.10 or newer
- Windows: MSVC / Visual Studio Build Tools is the most direct setup
- Linux/macOS: GCC or Clang should work with the same CMake project

---

## Build instructions

From the project root:

```bash
cmake -S . -B build
cmake --build build --config Debug
```

If you want to generate a Visual Studio solution explicitly on Windows:

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

The executable will be created under:

```bash
build/Debug/vortex.exe
```

---

## Running the application

Start the CLI with:

```bash
./build/Debug/vortex.exe
```

On Windows PowerShell or Command Prompt:

```powershell
.\build\Debug\vortex.exe
```

### Startup behavior

At launch, the program checks whether cached index files already exist:

- If `vortex_index.txt` and `vortex_paths.txt` are present, it asks:
  - `L` to load the cache
  - `R` to rebuild from a folder

If you choose rebuild, it will prompt for a directory path and recursively crawl that folder.

### Ignored directories
The crawler skips common large or irrelevant folders such as:

- `.git`
- `node_modules`
- `build`
- `Debug`
- `Release`
- `dist`

### Supported file types
The crawler includes common text-based files such as:

- `.txt`
- `.md`
- `.cpp`
- `.hpp`
- `.h`
- `.json`
- `.csv`
- `.xml`

---

## Typical usage workflow

1. Run the app.
2. Enter a directory path to crawl and index.
3. Wait for the crawl to finish and cache to be written.
4. Enter a search query at the `Search>` prompt.
5. Review ranked results and autocomplete suggestions.
6. Type `exit` or `quit` to end the session.

Example interaction:

```text
--- VORTEX INITIALIZATION ---
Enter full folder path to index: C:/Users/me/Documents
Crawling filesystem (this may take a moment)...
Cache successfully saved to disk!

--- VORTEX SEARCH ENGINE ---
Type 'exit' to quit.

Search> machine learning
```

The app prints ranked document matches and then shows prefix suggestions based on the final token in the query.

---

## Notes and limitations

- This is a local, offline search tool; it does not expose a REST API or web frontend.
- The crawler is text-focused and intentionally skips common generated folders.
- The current implementation is a research-grade prototype rather than a production-grade retrieval service.
- Cached index files are written to the working directory where the app is launched.

---

## Roadmap status

The codebase already includes core functionality for:

- [x] directory crawling
- [x] inverted index storage and retrieval
- [x] trie-based autocomplete
- [x] cosine ranking
- [x] fuzzy term matching
- [x] persistent cache files

Planned future work may include:

- [ ] richer file-type handling
- [ ] API integration layer
- [ ] larger-scale indexing optimizations
- [ ] better result presentation and filtering

---

## Author

Souvik Shomenath Dutta

- GitHub: [@s7d4007](https://github.com/s7d4007)
- LinkedIn: [Souvik Dutta](https://www.linkedin.com/in/souvikdutta7/)

> “Optimizing systems from the hardware up.”