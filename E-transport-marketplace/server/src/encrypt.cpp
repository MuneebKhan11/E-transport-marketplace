#include <iostream>
#include <string>

using namespace std;


const char SECRET_KEY = 'X';

string encryptString(const string& input) {
    std::string output = input;
    for (char& c : output) {
        c ^= SECRET_KEY;
    }
    return output;
}

std::string decryptString(const std::string& input) {
    std::string output = input;
    for (char& c : output) {
        c ^= SECRET_KEY;
    }
    return output;
}

