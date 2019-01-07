// Netanel Hugi, 203553490.

#ifndef QUERY_H
#define QUERY_H

#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

using namespace std;

// abstract class
class Query_base
{
  friend std::ostream& print(std::ostream&, const QueryResult&);

  public:
    using line_no = TextQuery::line_no;
    Query_base() = default;
    virtual ~Query_base() = default;
    virtual QueryResult eval(const TextQuery &) const = 0;
  
    //private:
};

class WordQuery : public Query_base
{
  public:
    // Default constructor:
    WordQuery(){
        query_word = "null";
    }
    WordQuery(const std::string &s) : query_word(s) {}
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }
    std::string rep() const { return query_word; }

  private:
    std::string query_word;
};

class AndQuery : public Query_base
{
  public:
    // Default constructor:
    AndQuery():s1("null"),s2("null"){
        
    }
    AndQuery(const std::string &left_string, const std::string &right_string) : left_query(WordQuery(left_string)), right_query(WordQuery(right_string)),
                                                                                s1(left_string), s2(right_string) {}
    QueryResult eval(const TextQuery &) const;

  private:
    WordQuery left_query, right_query;
    const std::string s1, s2;
};

class OrQuery : public Query_base
{
  public:
    OrQuery(const std::string &left_string, const std::string &right_string) : left_query(WordQuery(left_string)), right_query(WordQuery(right_string)),
                                                                               s1(left_string), s2(right_string) {}
    QueryResult eval(const TextQuery &) const;

  private:
    WordQuery left_query, right_query;
    const std::string s1, s2;
};

//WORD1 {NUMBER} WORD2 - Query:
//Returns the times that the words appear in the same lines,
// but do not separate them from more than 2 words 
class NQuery: public AndQuery
{
  private:
  // left_query,s1: WORD1
  // right_query.s2: WORD2
  // number: Separates words
    WordQuery left_query, right_query;
    string s1, s2;
    int number;

  public:
    NQuery(const string &word1, const string &word2, int n){
        number = n;
        left_query = WordQuery(word1); 
        right_query = WordQuery(word2);
        s1 = word1;
        s2 = word2;
    }
    QueryResult eval(const TextQuery &) const;
};

#endif
