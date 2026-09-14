#include "../include/inverted_index.hpp"
#include <sstream>
#include<cmath>
#include<algorithm>

void InvertedIndex::add_document(int doc_id, const std::string& text) {
    total_docs++; // Increases by 1 every time a document is indexed
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

double InvertedIndex::calculate_idf(const std::string& term) {
    if (index.find(term) == index.end()) {
        return 0.0;
    }
    
    double size = index[term].size();
    return std::log(total_docs / size);
}

std::vector<SearchResult> InvertedIndex::ranked_search(const std::string& term) {
    std::vector<SearchResult> results;
    
    // 1. Get the raw postings (gives the frequencies)
    std::vector<Posting> postings = search_term(term);
    
    // 2. Get the IDF multiplier for this specific word
    double idf = calculate_idf(term);
    
    // 3. Loop through every posting
    for (const auto& p : postings) {
        double  final_score =  p.frequency*idf;
        results.push_back({p.doc_id, final_score});
    }
    
    // 4. Sort the results in descending order (highest score first)
    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.score > b.score;
    });
    
    return results;
}