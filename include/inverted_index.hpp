#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <string>
#include <vector>
#include <unordered_map>

struct Posting {
    int doc_id;
    double frequency;
};

struct SearchResult{
    int doc_id;
    double score;
};

class InvertedIndex {
private:
    std::unordered_map<std::string, std::vector<Posting>> index;
    int total_docs=0;
    double calculate_idf(const std::string& term);

public:
    void add_document(int doc_id, const std::string& text);
    std::vector<Posting> search_term(const std::string& term);
    std::vector<SearchResult>  ranked_search(const std::string& term);
    std::unordered_map<std::string, double> get_query_vector(const std::string& query);
    std::vector<SearchResult> cosine_search(const std::string& query);
};

#endif // INVERTED_INDEX_HPP