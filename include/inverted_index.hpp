#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <string>
#include <vector>
#include <unordered_map>

struct Posting {
    int doc_id;
    double frequency;
};

class InvertedIndex {
private:
    std::unordered_map<std::string, std::vector<Posting>> index;
    int total_docs=0;

public:
    void add_document(int doc_id, const std::string& text);
    std::vector<Posting> search_term(const std::string& term);
};

#endif // INVERTED_INDEX_HPP