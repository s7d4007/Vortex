#include "../include/inverted_index.hpp"

void InvertedIndex::add_term(const std::string& term, int doc_id, double term_freq){
    index[term].push_back({doc_id, term_freq});
};

std::vector<Posting> InvertedIndex::search_term(const std::string& term) {
    auto it = index.find(term);
    if(it != index.end()){
        return it->second;
    }
    return{};
}