#include "../include/utils.hpp"
#include "../include/trie.hpp"

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& word) {
    std::string normalized_word = normalize_text(word);
    TrieNode* current = root;
    for (char ch : normalized_word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->is_end_of_word = true;
}

bool Trie::search(const std::string& word) {
    std::string normalized_word = normalize_text(word);
    TrieNode* current = root;
    for (char ch : normalized_word) {
        if (current->children.find(ch) == current->children.end()) {
            return false;
        }
        current = current->children[ch];
    }
    return current->is_end_of_word;
}

bool Trie::starts_with(const std::string& prefix) {
    std::string normalized_prefix = normalize_text(prefix);
    TrieNode* current = root;
    for (char ch : normalized_prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return false;
        }
        current = current->children[ch];
    }
    return true;
}

void Trie::dfs(TrieNode* node, std::string current_word, std::vector<std::string>& results) {
    if (node->is_end_of_word) {
        results.push_back(current_word);
    }
    
    for (auto& pair : node->children) {
        dfs(pair.second, current_word + pair.first, results);
    }
}

std::vector<std::string> Trie::get_words_with_prefix(const std::string& prefix) {
    std::string normalized_prefix = normalize_text(prefix);
    std::vector<std::string> results;
    TrieNode* current = root;
    
    for (char ch : normalized_prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return results;
        }
        current = current->children[ch];
    }
    
    dfs(current, normalized_prefix, results);
    return results;
}