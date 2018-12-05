#include "parsing.h"
#include "interface.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parse_int(const string& s) {
    // parse the entire string into an int
    int result = 0;

    // convert string into numbers
    for(char each:s) {
        int a = each - '0';
        if (a >= 0 && a<=9) {
            result = result*10 + a;
        }

            // throw error if the string contains other syntax
        else {
            throw runtime_error("string must be nonnegative integers");
        }
    }
    return result;
}

vector<string> tokenize(const string& line)
// tokenize the input string while recognizing double quotes
{
    vector<string> tokens;
    int i;

    if (line.at(0)!= '\"') {
        tokens.push_back("");
        i = 0;
    }

    else {
        i = -1;
    }

    bool quote = false;
    bool space = false;

    for(char each:line) {
        // if the loop scans through a double quote
        if(each == '\"') {
            quote = !quote;
            if(space == false) {
                i++;
                tokens.push_back("");
            }
            space = false;
        }

            // if the loop scans through space
        else if(each == ' ' && quote == false) {
            if(!tokens[i].empty()) {
                i++;
                tokens.push_back("");
            }
            space = true;
        }

            // store the string in groups
        else {

            tokens[i].push_back(each);
            space = false;
        }
    }
    // throw error when there is no ending double quote
    if (quote == true) {
        throw std::runtime_error("Double quotes not in pair!");
    }
    else {
        return tokens;
    }
}
