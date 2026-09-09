#ifndef TRIE_HPP
#define TRIE_HPP

#include <unordered_map>
#include <string>
#include <vector>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool is_end_of_word = false;
};

class Trie {
private:
    TrieNode* root;

public:
    Trie();
    void insert(const std::string& word);
    bool search(const std::string& word);
    bool starts_with(const std::string& prefix);
};

#endif // TRIE_HPP