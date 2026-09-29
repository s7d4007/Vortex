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
#include <thread>
#include <mutex>
#include <vector>

std::mutex vortex_mutex;

namespace fs = std::filesystem;

std::unordered_map<int, std::string> document_paths;
int current_doc_id = 1;

void save_paths(const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& [id, path] : document_paths) {
        out << id << "|" << path << "\n";
    }
}

void load_paths(const std::string& filename) {
    std::ifstream in(filename);
    std::string line;
    int paths_loaded = 0;
    
    while (std::getline(in, line)) {
        size_t delim = line.find('|');
        if (delim != std::string::npos) {
            document_paths[std::stoi(line.substr(0, delim))] = line.substr(delim + 1);
        }
        
        if (++paths_loaded % 1000 == 0) {
            std::cout << "\rLoading paths: " << paths_loaded << " files mapped..." << std::flush;
        }
    }
    std::cout << "\rLoading paths: " << paths_loaded << " files mapped. Complete!        \n";
}

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

/* Isolate file operations and string tokenization */
void process_file(const std::string& file_path, int doc_id, InvertedIndex& engine, Trie& autocomplete) {
    std::ifstream file(file_path);
    if (!file.is_open()) return;

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + " ";
    }
    file.close();

    std::vector<std::string> local_tokens;
    std::string normalized_content = normalize_text(content);
    std::stringstream ss(normalized_content);
    std::string word;
    while (ss >> word) {
        if (!is_stop_word(word)) {
            local_tokens.push_back(word);
        }
    }

    /* Synchronize core data structure mutation */
    std::lock_guard<std::mutex> lock(vortex_mutex);
    engine.add_document(doc_id, content);
    for (const auto& token : local_tokens) {
        autocomplete.insert(token);
    }
}

void crawl_directory(const std::string& directory_path, InvertedIndex& engine, Trie& autocomplete) {
    std::unordered_set<std::string> text_extensions = {
        ".txt", ".md", ".cpp", ".hpp", ".h", ".json", ".csv", ".xml"
    };
    std::unordered_set<std::string> ignore_dirs = {
        "node_modules", ".git", "build", "Debug", "Release", "dist"
    };
    
    std::vector<std::thread> workers;
    unsigned int max_threads = std::thread::hardware_concurrency();

    auto it = fs::recursive_directory_iterator(directory_path, fs::directory_options::skip_permission_denied);
    auto end = fs::recursive_directory_iterator();

    while (it != end) {
        if (it->is_directory()) {
            std::string dir_name = it->path().filename().string();
            if (ignore_dirs.count(dir_name)) {
                it.disable_recursion_pending(); // Instantly bypasses the entire folder
            }
        } else if (it->is_regular_file()) {
            std::string file_path = it->path().string();
            std::string filename = it->path().filename().string();
            std::string extension = it->path().extension().string();

            // Dynamic progress rewrite
            if (current_doc_id % 25 == 0) {
                std::cout << "\rIndexed " << current_doc_id << " files..." << std::flush;
            }

            {
                std::lock_guard<std::mutex> lock(vortex_mutex);
                engine.add_document(current_doc_id, filename);
                
                std::string normalized_filename = normalize_text(filename);
                std::stringstream fs_ss(normalized_filename);
                std::string f_word;
                while (fs_ss >> f_word) {
                    if (!is_stop_word(f_word)) autocomplete.insert(f_word);
                }
                document_paths[current_doc_id] = file_path;
            }

            if (text_extensions.find(extension) != text_extensions.end()) {
                workers.emplace_back(process_file, file_path, current_doc_id, std::ref(engine), std::ref(autocomplete));
            }
            
            if (workers.size() >= max_threads) {
                for (auto& w : workers) { if (w.joinable()) w.join(); }
                workers.clear();
            }
            current_doc_id++;
        }
        
        std::error_code ec;
        it.increment(ec);
    }

    for (auto& w : workers) { if (w.joinable()) w.join(); }
    std::cout << "\rIndexed " << (current_doc_id - 1) << " files. Crawl complete!        \n";
}

int main() {
    InvertedIndex engine;
    Trie autocomplete;

    std::cout << "\n--- VORTEX INITIALIZATION ---\n";
    bool run_crawl = true;

    if (fs::exists("vortex_index.txt") && fs::exists("vortex_paths.txt")) {
        std::cout << "Existing cache found. [L]oad cache or [R]ebuild from folder? (L/R): ";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice == "L" || choice == "l") run_crawl = false;
    }

    InvertedIndex disk_engine;

    if (run_crawl) {
        std::cout << "Enter full folder path to index: ";
        std::string target_dir;
        std::getline(std::cin, target_dir);

        if (!target_dir.empty() && fs::exists(target_dir)) {
            std::cout << "Crawling filesystem (this may take a moment)...\n";
            crawl_directory(target_dir, engine, autocomplete);
            
            engine.save_index("vortex_index.txt");
            save_paths("vortex_paths.txt");
            std::cout << "Cache successfully saved to disk!\n";
            
            disk_engine = engine; // Use memory engine directly
        }
    } else {
        std::cout << "Loading engine from disk...\n";
        disk_engine.load_index("vortex_index.txt");
        load_paths("vortex_paths.txt");
        
        // Rebuild autocomplete Trie from disk data with progress
        std::vector<std::string> all_terms = disk_engine.get_all_terms();
        int term_count = 0;
        for (const auto& term : all_terms) {
            autocomplete.insert(term);
            
            if (++term_count % 5000 == 0) {
                std::cout << "\rRebuilding Trie: " << term_count << " / " << all_terms.size() << " terms..." << std::flush;
            }
        }
        std::cout << "\rRebuilding Trie: " << term_count << " / " << all_terms.size() << " terms. Complete!        \n";
        std::cout << "Load complete!\n";
    }

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
            int result_count = 0;
            std::unordered_map<std::string, int> dir_counts;
            
            for (const auto& res : phrase_results) {
                if (result_count >= 10) break; 
                
                std::string path = document_paths.count(res.doc_id) ? document_paths[res.doc_id] : "Unknown Document";
                std::string parent_dir = fs::path(path).parent_path().string();
                
                /* Restrict output to a maximum of 2 files per directory to diversify results */
                if (dir_counts[parent_dir] >= 2) continue;
                
                dir_counts[parent_dir]++;
                std::cout << "Score: " << res.score << " | File: " << path << "\n";
                result_count++;
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