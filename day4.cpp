
// Example input
/* 
2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8 
*/

#include <iostream>
#include <fstream>
#include <string>

const std::string inputFilePath = "day4.txt";

bool isPairContainedWithinOtherPair(std::pair<int, int> p1, std::pair<int, int> p2){
    return ((p1.first >= p2.first) && (p1.second <= p2.second));
}

bool isThereOverlap(std::pair<int, int> p1, std::pair<int, int> p2){
    for(int i=p1.first; i <= p1.second; i++){
        if((i >= p2.first) && (i <= p2.second)){
            return true;
        }
    }
    return false;
}

int main(){
    int containedAssignments = 0;
    int overlapAssignments = 0;

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    while(std::getline(ifs, line)){
        
        // Parse input
        std::pair<int, int> elf1Assignment;
        std::pair<int, int> elf2Assignment;

        int elf1LoweBound = std::stoi(line.substr(0, line.find('-')));
        int elf1UpperBound = std::stoi(line.substr(line.find('-') + 1, line.find(',')));
        
        int elf2LoweBound = std::stoi(line.substr(line.find(',') + 1, line.rfind('-')));
        int elf2UpperBound = std::stoi(line.substr(line.rfind('-') + 1, line.length()));

        elf1Assignment = {elf1LoweBound, elf1UpperBound};
        elf2Assignment = {elf2LoweBound, elf2UpperBound};

        // Ex1
        if ((isPairContainedWithinOtherPair(elf1Assignment, elf2Assignment)) || (isPairContainedWithinOtherPair(elf2Assignment, elf1Assignment))){
            containedAssignments++;
        }

        // Ex2
        if (isThereOverlap(elf1Assignment, elf2Assignment)){
            overlapAssignments++;
        }
    }

    std::cout << "--Ex1 Output: " << containedAssignments << std::endl;
    std::cout << "--Ex2 Output: " << overlapAssignments << std::endl;

    return 0;
}