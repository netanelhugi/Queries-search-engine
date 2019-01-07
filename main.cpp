// Netanel Hugi, 203553490.

// This file contains code from "C++ Primer", 
// by Stanley B.Lippman, Jose Lajoie, and Barbara E. Moo
#include "Query.h"
#include "TextQuery.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "No input file" << endl;
		return EXIT_FAILURE;
	}
	ifstream infile(argv[1]);
	if (!infile)
	{
		cerr << "No input file!" << endl;
		return EXIT_FAILURE;
	}
	TextQuery tq(infile); // store the file in a vector and build the query map
						  	// tq.display_map(); // debugging aid

	// Regular expressions:
	regex ANDpattern(R"((\w+) \{AND\} (\w+)$)");
	regex ORpattern(R"((\w+) \{OR\} (\w+)$)");
	regex WORDpattern(R"(^\b\w*\b$)");
	regex Npattern(R"((\w+) \{(\d+)\} (\w+)$)");
	regex RANGEpattern(R"(([\s\S]+)\[(\d+),(\d+)]$)");

	smatch resultPattern;


	while (true)
	{
		cout << "Enter one or two words to look for, or q to quit:\n";
		string line;
		getline(cin, line); //Get a query.
		if (line.empty() || line == "q")
			break;

		//WORD1 {AND} WORD2 - Query:
		if (regex_search(line, resultPattern, ANDpattern))
		{ 
			AndQuery aquery(resultPattern[1], resultPattern[2]);
			auto result = aquery.eval(tq);
			print(cout, result) << endl;
		}
		//WORD1 {OR} WORD2 - Query:
		else if (regex_search(line, resultPattern, ORpattern))
		{
			OrQuery aquery(resultPattern[1], resultPattern[2]);
			auto result = aquery.eval(tq);
			print(cout, result) << endl;
		}
		//WORD1 {NUMBER} WORD2 - Query:
		//Returns the times that the words appear in the same lines,
		// but do not separate them from more than 2 words 
		else if (regex_search(line, resultPattern, Npattern))
		{
			int n = stoi(resultPattern[2]);
			NQuery aquery(resultPattern[1], resultPattern[3], n);
			auto result = aquery.eval(tq);
			print(cout, result) << endl;
		}

		//WORD - Query:
		else if (regex_search(line, resultPattern, WORDpattern))
		{
			WordQuery aquery(resultPattern[0]);
			auto result = aquery.eval(tq);
			print(cout, result) << endl;
		}

		//[lineStart,lineEnd] - Query:
		//Return results between this two lines.
		else if (regex_search(line, resultPattern, RANGEpattern))
		{
			string qu = resultPattern[1]; //get the query.

			int start = stoi(resultPattern[2]); //get the start line.
			int end = stoi(resultPattern[3]); //get the end line.

			// Printing error if the start number is larget than the end.
			if (start > end)
			{
				// throw out_of_range("The start index is larger than the end index.");
				cout << "Error: The start index is larger than the end index." << endl;
			}
			else
			{

				// Regular expressions:
				regex ANDpattern2(R"((\w+) \{AND\} (\w+))");
				regex ORpattern2(R"((\w+) \{OR\} (\w+))");
				regex Npattern2(R"((\w+) \{(\d+)\} (\w+))");
				regex WORDpattern2(R"(^\b\w*\b)");

				//WORD1 {AND} WORD2 [s,e] - Query:
				if (regex_search(qu, resultPattern, ANDpattern2))
				{
					AndQuery aquery(resultPattern[1], resultPattern[2]);
					auto result = aquery.eval(tq);
					print(cout, result,start,end) << endl;
				}
				//WORD1 {OR} WORD2 [s,e] - Query:
				else if (regex_search(qu, resultPattern, ORpattern2))
				{
					OrQuery aquery(resultPattern[1], resultPattern[2]);
					auto result = aquery.eval(tq);
					print(cout, result,start,end) << endl;
				}
				//WORD1 {NUMBER} WORD2 [s,e] - Query:
				else if (regex_search(qu, resultPattern, Npattern2))
				{
					int n = stoi(resultPattern[2]);
					NQuery aquery(resultPattern[1], resultPattern[3],n);
					auto result = aquery.eval(tq);
					print(cout, result,start,end) << endl;
				}
				//WORD [s,e] - Query:
				else if (regex_search(qu, resultPattern, WORDpattern2))
				{
					WordQuery aquery(resultPattern[0]);
					auto result = aquery.eval(tq);
					print(cout, result,start,end) << endl;
				}

			}

		}
		// If enter a wrong query:
		else{
			cout << "Unrecognized search \n" << endl;
		}

	}
	return 0;
}
