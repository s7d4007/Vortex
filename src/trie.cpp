#include "../include/trie.hpp"

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->is_end_of_word = true;
}

bool Trie::search(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            return false; // Path broken
        }
        current = current->children[ch];
    }
    return current->is_end_of_word; // Only true if it's a complete word
}

bool Trie::starts_with(const std::string& prefix) {
    TrieNode* current = root;
    for (char ch : prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return false; // Path broken
        }
        current = current->children[ch];
    }
    return true; // Reached the end of the prefix successfully
}