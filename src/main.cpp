#include <iostream>
#include "../include/inverted_index.hpp"

int main() {
    InvertedIndex engine;

    engine.add_term("algorithm", 1, 1.5);
    engine.add_term("algorithm", 2, 0.8);
    engine.add_term("search", 1, 2.1);

    std::vector<Posting> results = engine.search_term("algorithm");

    for (const auto& p : results) {
        std::cout << p.doc_id << " " << p.term_frequency << "\n";
    }

    return 0;
}