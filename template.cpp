#include <iostream>
#include <fstream>
#include <string>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "dayX.txt";

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    while(std::getline(ifs, line)){
        
    }

    std::cout << "--Ex1 Output: " << "" << std::endl;
    std::cout << "--Ex2 Output: " << "" << std::endl;

    return 0;
}