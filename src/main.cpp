#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include "inverted_index.hpp"
#include "trie.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

std::unordered_map<int, std::string> document_paths;
int current_doc_id = 1;

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

void crawl_directory(const std::string& directory_path, InvertedIndex& engine, Trie& autocomplete) {
    std::unordered_set<std::string> text_extensions = {
        ".txt", ".md", ".cpp", ".hpp", ".h", ".json", ".csv", ".xml"
    };

    for (const auto& entry : fs::recursive_directory_iterator(directory_path, fs::directory_options::skip_permission_denied)) {
        if (entry.is_regular_file()) {
            std::string file_path = entry.path().string();
            std::string filename = entry.path().filename().string();
            std::string extension = entry.path().extension().string();
            std::cout << "Indexing: " << filename << "\n";

            /* Index filename for universal file discovery */
            engine.add_document(current_doc_id, filename);

            /* Tokenize filename for Trie autocomplete */
            std::string normalized_filename = normalize_text(filename);
            std::stringstream fs_ss(normalized_filename);
            std::string f_word;
            while (fs_ss >> f_word) {
                if (!is_stop_word(f_word)) autocomplete.insert(f_word);
            }

            /* Read text formats and populate engine components */
            if (text_extensions.find(extension) != text_extensions.end()) {
                std::ifstream file(file_path);
                if (file.is_open()) {
                    std::string content;
                    std::string line;
                    while (std::getline(file, line)) {
                        content += line + " ";
                    }
                    file.close();

                    engine.add_document(current_doc_id, content);
                    
                    /* Populate Trie with normalized file content */
                    std::string normalized_content = normalize_text(content);
                    std::stringstream ss(normalized_content);
                    std::string word;
                    while (ss >> word) {
                        if (!is_stop_word(word)) {
                            autocomplete.insert(word);
                        }
                    }
                }
            }

            document_paths[current_doc_id] = file_path;
            current_doc_id++;
        }
    }
}

int main() {
    InvertedIndex engine;
    Trie autocomplete;

    std::cout << "\n--- VORTEX INITIALIZATION ---\n";
    std::cout << "Enter full folder path to index (or press Enter to skip): ";
    std::string target_dir;
    std::getline(std::cin, target_dir);

    if (!target_dir.empty() && fs::exists(target_dir)) {
        std::cout << "Crawling filesystem (this may take a moment)...\n";
        
        crawl_directory(target_dir, engine, autocomplete);
        
        std::cout << "Crawling complete. Indexed " << (current_doc_id - 1) << " files.\n";
    } else if (!target_dir.empty()) {
        std::cout << "Directory not found. Skipping crawl.\n";
    }

    /* Save index to disk */
    engine.save_index("vortex_index.txt");
    std::cout << "Index successfully saved to vortex_index.txt!\n";

    /* Load index from disk into secondary engine instance */
    InvertedIndex disk_engine;
    disk_engine.load_index("vortex_index.txt");

    std::string input;
    std::cout << "\n--- VORTEX SEARCH ENGINE ---\nType 'exit' to quit.\n\n";

    while (true) {
        std::cout << "Search> ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit") break;
        if (input.empty()) continue;

        /* Execute search against disk_engine */
        std::vector<SearchResult> phrase_results = disk_engine.cosine_search(input);
        
        std::cout << "\n[ Phrase Matches ]\n";
        if (phrase_results.empty()) {
            std::cout << "No documents found.\n";
        } else {
            for (const auto& res : phrase_results) {
                std::string path = document_paths.count(res.doc_id) ? document_paths[res.doc_id] : "Unknown Document";
                std::cout << "Score: " << res.score << " | File: " << path << "\n";
            }
        }

        /* Extract final token for prefix matching */
        std::string normalized_input = normalize_text(input);
        std::stringstream ss(normalized_input);
        std::string last_word;
        while (ss >> last_word) {} 
        
        std::vector<std::string> suggestions = autocomplete.get_words_with_prefix(last_word);
        std::cout << "\n[ Autocomplete Suggestions ]\n";
        if (suggestions.empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& word : suggestions) {
                std::cout << "- " << word << "\n";
            }
        }
        std::cout << "----------------------------\n";
    }

    return 0;
}