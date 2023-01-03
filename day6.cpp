// Example input: mjqjpqmgbljsphdztnvjfqwrcgsmlb - Output: 5 (Ex1)

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

const std::string inputFilePath = "inputs/day6.txt";

bool doesStringHaveUniqueChars(std::string str){
    for(int i=0; i < str.length();i++){
        for(int j=i+1; j < str.length();j++){
            if (str[i] == str[j]){
                return false;
            }
        }
    }
    return true;
}

int charsToUniqueMessage(std::string input, int charsToCheck){
    int charCount = charsToCheck;
    for (int i = charsToCheck; i < input.length(); i++)
    {
        if (doesStringHaveUniqueChars(input.substr(i - charsToCheck, charsToCheck)))
        {
            break;
        }
        charCount++;
    }
    return charCount;
}

int main(){

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string inputStr = buffer.str();

    int ex1Chars = charsToUniqueMessage(inputStr, 4);
    int ex2Chars = charsToUniqueMessage(inputStr, 14);

    std::cout << "--Ex1 Output: " << ex1Chars << std::endl;
    std::cout << "--Ex2 Output: " << ex2Chars << std::endl;
    
    return 0;
}