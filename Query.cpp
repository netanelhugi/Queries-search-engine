// Netanel Hugi, 203553490.

#include "Query.h"
#include "TextQuery.h"
#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <regex>
using namespace std;

QueryResult
AndQuery::eval(const TextQuery &text) const
{
    auto left = left_query.eval(text), right = right_query.eval(text);
    // set to hold the intersection of left and right
    auto ret_lines = make_shared<set<line_no>>();
    // writes the intersection of two ranges to a destination iterator
    set_intersection(left.begin(), left.end(),
                     right.begin(), right.end(),
                     inserter(*ret_lines, ret_lines->begin()));

    return QueryResult(s1 + " {AND} " + s2, ret_lines, left.get_file());
}

QueryResult
OrQuery::eval(const TextQuery &text) const
{
    auto left = left_query.eval(text), right = right_query.eval(text);
    // copy the line numbers from the left-hand operand into the result set
    auto ret_lines =
        make_shared<set<line_no>>(left.begin(), left.end());
    // insert lines from the right-hand operand
    ret_lines->insert(right.begin(), right.end());
    // return the new QueryResult representing the union of lhs and rhs
    return QueryResult(s1 + " {OR} " + s2, ret_lines, left.get_file());
}

ostream &print(ostream &os, const QueryResult &qr)
{
    // print each line in which the word appeared
    for (auto num : *qr.lines) // for every element in the set
        os << "(line " << num + 1 << ") "
           << *(qr.file->begin() + num) << endl;
    // print the count and all occurrences
    os << "\"" << qr.sought << "\" occurs " << qr.lines->size() << " times" << endl;
    return os;
}

// Overload "print" function
// for [lineStart,lineEnd] query:
ostream &print(ostream &os, const QueryResult &qr, size_t m, size_t n)
{
    int count = 0; // Count the lines in the range.
    
    for (auto num : *qr.lines) // for every element in the set.

        // Check if the line number is in the range.
        if (num >= m-1 && num <= n-1)
        {
            count++;
            os << "(line " << num + 1 << ") "
               << *(qr.file->begin() + num) << endl;
        }

    // print the count and all occurrences
    os << "\"" << qr.sought << "\" occurs " << count << " times" << endl;
    return os;
}

QueryResult NQuery::eval(const TextQuery &text) const
{
    // nQuery regular expression:
    regex Npattern(R"([\w\s]*\b)" + s1 + R"(\b\s(\w*\s){0,)" + to_string(number) + R"(}\b)" + s2 + R"(\b[\s\w]*)");

    auto left = left_query.eval(text), right = right_query.eval(text);
    // set to hold the intersection of left and right
    auto ret_lines = make_shared<set<line_no>>();
    // writes the intersection of two ranges to a destination iterator
    set_intersection(left.begin(), left.end(),
                     right.begin(), right.end(),
                     inserter(*ret_lines, ret_lines->begin()));

    QueryResult qr(s1 + " {N} " + s2, ret_lines, left.get_file());

    auto Nlines = make_shared<set<line_no>>();
    smatch resultPattern;

    // Run over the original AND query result,
    // if between the two words do not separate more than N words,
    // enter the line to "Nline" Set.
    for (set<line_no>::iterator itr = qr.begin(); itr != qr.end(); itr++)
    {
        string str = *(qr.get_file()->begin() + *itr);

        if (regex_search(str, resultPattern, Npattern) > 0)
        {
            Nlines->insert(*itr);
        }
    }

    return QueryResult(s1 + " {" + to_string(number) + "} " + s2, Nlines, left.get_file());
}
