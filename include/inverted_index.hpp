#include<string>
#include<vector>
#include<unordered_map>

struct Posting{
    int doc_id;
    double term_frequency;


};

class InvertedIndex {
  private:
  std::unordered_map<std::string, std::vector<Posting>> index;
  
  public:
  void add_term(const std::string& term , int doc_id , double term_freq);
  std::vector<Posting> search_term(const std::string& term);
};