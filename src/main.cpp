#include <iostream>
#include "inverted_index.hpp"
#include "trie.hpp"

int main() {
    InvertedIndex engine;
    engine.add_document(1, "algorithm search engineering");
    engine.add_document(2, "algorithm data structures");

    Trie autocomplete;
    autocomplete.insert("algorithm");
    autocomplete.insert("algorithmic");
    autocomplete.insert("search");

    std::cout << "Searching 'algorithm': " << (autocomplete.search("algorithm") ? "Found" : "Not Found") << std::endl;
    std::cout << "Prefix 'alg' exists: " << (autocomplete.starts_with("alg") ? "Yes" : "No") << std::endl;
    std::cout << "Prefix 'cat' exists: " << (autocomplete.starts_with("cat") ? "Yes" : "No") << std::endl;

    return 0;
}