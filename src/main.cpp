#include <iostream>
#include "inverted_index.hpp"
#include "trie.hpp"

int main() {
    InvertedIndex engine;
    engine.add_document(1, "algorithm search engineering");
    engine.add_document(2, "algorithm data structures");
    engine.add_document(3, "C++ algorithm algorithm");
    engine.add_document(4, "Learning Python Algorithm");

    Trie autocomplete;
    autocomplete.insert("algorithm");
    autocomplete.insert("algorithmic");
    autocomplete.insert("search");

    std::cout << "Searching 'algorithm': " << (autocomplete.search("algorithm") ? "Found" : "Not Found") << std::endl;
    std::cout << "Prefix 'alg' exists: " << (autocomplete.starts_with("alg") ? "Yes" : "No") << std::endl;
    std::cout << "Prefix 'cat' exists: " << (autocomplete.starts_with("cat") ? "Yes" : "No") << std::endl;

    engine.add_document(3, "algorithm c++ algorithm fast");
    engine.add_document(4, "learning python data");

    std::vector<SearchResult> results = engine.ranked_search("algorithm");

    std::cout << "\nRanked Search Results for 'algorithm':\n";
    for (const auto& res : results) {
        std::cout << "Document ID: " << res.doc_id << " | Score: " << res.score << "\n";
    }

    return 0;
}