using namespace std;
#include <iostream>
#include <regex>





int main(int argc, char const *argv[])
{
    string s1 = "ran";
    string s2 = "Digital";
    int number  = 1;

    string text = "(circa 1969-70) ran on the Digital Equipment Corporation PDP-7 and -9 computers.";
	smatch resultPattern;

    // regex Npattern(R"(\b)" + s1 + R"(\b\s(\w*\s){0,)"+ to_string(number) + R"(}\b)" + s2+ R"(\b)");
    // regex Npattern(R"(\b)" + s1 + R"(\b\s(\w*\s){0,10})" + s2);

    regex Npattern(R"([\w\s]*\b)" + s1 + R"(\b\s(\w*\s){0,)" + to_string(number) + R"(}\b)" + s2 + R"(\b[\s\w]*)");


    // regex Npattern(R"([\s\S]*\b)"+ s1 + R"(\b)");

    cout << regex_search(text, resultPattern, Npattern) << endl;


    if(regex_search(text, resultPattern, Npattern)>0 ){

        cout << "test!!" << endl;

    }


    
}
