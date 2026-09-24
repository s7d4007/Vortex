#include "../include/utils.hpp"
#include "../include/inverted_index.hpp"
#include <sstream>
#include<cmath>
#include<algorithm>

void InvertedIndex::add_document(int doc_id, const std::string& text) {
    total_docs++; // Increases by 1 every time a document is indexed
    std::stringstream ss(text);
    std::string word;
    
    while (ss >> word) {
            word = normalize_text(word);
            
            auto& postings = index[word];
            bool found = false;
            for (auto& p : postings) {
                if (p.doc_id == doc_id) {
                    p.frequency += 1.0;
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
    
    double size = static_cast<double>(index[term].size());
    return std::log(total_docs / size) + 1.0;
}

std::vector<SearchResult> InvertedIndex::ranked_search(const std::string& term) {
    std::string normalized_term = normalize_text(term);
    std::string best_match = find_closest_term(normalized_term);
    
    std::vector<SearchResult> results;
    if (best_match.empty()) return results;
    
    double idf = calculate_idf(best_match);
    for (const auto& p : index[best_match]) {
        results.push_back({p.doc_id, p.frequency * idf});
    }
    
    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.score > b.score;
    });
    
    return results;
}

std::unordered_map<std::string, double> InvertedIndex::get_query_vector(const std::string& query) {
    std::unordered_map<std::string, double> query_vector;
    std::stringstream ss(query);
    std::string word;
    
    while (ss >> word) {
        word = normalize_text(word);
        std::string best_match = find_closest_term(word);
        if (!best_match.empty()) {
            query_vector[best_match] += 1.0;
        }
    }
    
    for (auto& pair : query_vector) {
        double idf = calculate_idf(pair.first);
        pair.second = pair.second * idf;
    }
    
    return query_vector;
}

std::vector<SearchResult> InvertedIndex::cosine_search(const std::string& query) {
    std::unordered_map<std::string, double> query_vector = get_query_vector(query);
    std::unordered_map<int, double> dot_products;
    std::unordered_map<int, double> doc_magnitudes;
    std::vector<SearchResult> results;

    double query_magnitude = 0.0;
    for (const auto& pair : query_vector) {
        query_magnitude += pair.second * pair.second;
    }
    query_magnitude = std::sqrt(query_magnitude);

    if (query_magnitude == 0.0) return results;

    for (const auto& q_pair : query_vector) {
        std::string word = q_pair.first;
        double q_weight = q_pair.second;

        std::vector<Posting> postings = search_term(word);
        double idf = calculate_idf(word);

        for (const auto& p : postings) {
            double doc_weight = p.frequency * idf;
            dot_products[p.doc_id] += q_weight * doc_weight;
            doc_magnitudes[p.doc_id] += doc_weight * doc_weight;
        }
    }

    for (const auto& doc : dot_products) {
        int doc_id = doc.first;
        double doc_mag = std::sqrt(doc_magnitudes[doc_id]);
        double score = doc.second / (query_magnitude * doc_mag);
        results.push_back({doc_id, score});
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.score > b.score;
    });

    return results;
}

std::string InvertedIndex::find_closest_term(const std::string& query_term, int max_distance) {
    if (index.find(query_term) != index.end()) {
        return query_term;
    }
    
    std::string best_match = "";
    int min_dist = max_distance + 1;
    
    for (const auto& pair : index) {
        int dist = levenshtein_distance(query_term, pair.first);
        if (dist < min_dist) {
            min_dist = dist;
            best_match = pair.first;
        }
    }
    
    return min_dist <= max_distance ? best_match : "";
}