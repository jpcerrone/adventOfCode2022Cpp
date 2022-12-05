
// Example input
/* vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw */

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <assert.h>

const std::string inputFilePath = "day3.txt";

// Calculates the type's value using the ASCII value from its char and the offset for it
int getTypePriority(char type){
    const int uppercaseOffset = 65 - 27;
    const int lowerCaseOffset = 97 - 1;
    if ((65 <= type) &&  (type<= 90)){ // upppercase
        return type - uppercaseOffset;// 27 to 52
    }
    else{ // lowercase 
        return type - lowerCaseOffset;// 1 to 26
    }
}

int calculateEx1(){

    int prioritiesSum = 0;

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string rucksack;
    while(std::getline(ifs, rucksack)){
        // split into compartments
        const int elements = rucksack.length();
        const std::string compartment1 = rucksack.substr(0, elements/2);
        const std::string compartment2 = rucksack.substr(elements/2, elements);
        
        // find common types
        std::set<char> commonTypes;
        for(int i=0; i < compartment1.length(); i++){
                for(int j=0; j < compartment2.length(); j++){
                    if(compartment1[i] == compartment2[j]){
                        commonTypes.insert(compartment1[i]);
                        break;
                    }
                }
        }

        // get types values
        for(std::set<char>::iterator it = commonTypes.begin(); it != commonTypes.end() ; it++){
            prioritiesSum += getTypePriority(*it);
        }

    }
    return prioritiesSum;
}

int calculateEx2(){

    int prioritiesSum = 0;
    int groupLineCounter = 0;

    std::set<char> possibleBadges;


    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string rucksack;
    while(std::getline(ifs, rucksack)){
        // If it's the first elf in the group add his whole rucksack to the possible badges set
        if (groupLineCounter % 3 == 0){
            for(int i=0; i < rucksack.length(); i++){
                possibleBadges.insert(rucksack[i]);
            }
        }
        else{
            // Remove badges that aren't repeated in the second and third elve's rucksacks from the possible badges set
            std::set<char> filteredSet(possibleBadges);
            for(std::set<char>::iterator it = possibleBadges.begin(); it != possibleBadges.end(); it++){
                if (rucksack.find(*it) == std::string::npos)
                    filteredSet.extract(*it);
            }
            possibleBadges = filteredSet;
            
            // After going through the whole group, sum the priority of the common badge to the total
            if (groupLineCounter % 3 == 2){ 
                assert(possibleBadges.size() == 1); // Check we only have one badge
                prioritiesSum += getTypePriority(*possibleBadges.begin());
                possibleBadges.clear();
            }
        }
        groupLineCounter++;
    }
    return prioritiesSum;
}

int main(){

    std::cout << "--Ex1 output: " << calculateEx1() << std::endl;
    std::cout << "--Ex2 output: " << calculateEx2() << std::endl;

    return 0;
}