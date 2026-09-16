#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "inverted_index.hpp"
#include "trie.hpp"

void autocomplete_search(const std::string& prefix, Trie& trie, InvertedIndex& engine) {
    std::vector<std::string> words = trie.get_words_with_prefix(prefix);
    std::unordered_map<int, double> combined_scores;
    
    for (const std::string& word : words) {
        std::vector<SearchResult> results = engine.ranked_search(word);
        for (const SearchResult& res : results) {
            combined_scores[res.doc_id] += res.score;
        }
    }
    
    std::vector<SearchResult> final_results;
    for (const auto& pair : combined_scores) {
        final_results.push_back({pair.first, pair.second});
    }
    
    std::sort(final_results.begin(), final_results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.score > b.score;
    });
    
    std::cout << "\nAggregated Search Results for prefix '" << prefix << "':\n";
    if (final_results.empty()) {
        std::cout << "No matching documents found.\n";
    } else {
        for (const auto& res : final_results) {
            std::cout << "Document ID: " << res.doc_id << " | Total Score: " << res.score << "\n";
        }
    }
}

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

    std::vector<std::string> suggestions = autocomplete.get_words_with_prefix("alg");
    std::cout << "\nAutocomplete suggestions for 'alg':\n";
    for (const auto& word : suggestions) {
    std::cout << "- " << word << "\n";
    }
    autocomplete_search("alg", autocomplete, engine);
    return 0;
}