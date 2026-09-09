#include "../include/inverted_index.hpp"
#include <sstream>

void InvertedIndex::add_document(int doc_id, const std::string& text) {
    std::stringstream ss(text);
    std::string word;
    
    while (ss >> word) {
        // Simple check to see if this doc_id already exists in the posting list for this word
        auto& postings = index[word];
        bool found = false;
        for (auto& p : postings) {
            if (p.doc_id == doc_id) {
                p.frequency += 1.0; // Increment frequency if word appears multiple times
                found = true;
                break;
            }
        }
        if (!found) {
            postings.push_back({doc_id, 1.0});
        }
    }
}

std::vector<Posting> InvertedIndex::search_term(const std::string& term) {
    if (index.find(term) != index.end()) {
        return index[term];
    }
    return {};
}