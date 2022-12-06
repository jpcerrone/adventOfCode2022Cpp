/*     
    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2 

*/
#include <iostream>
#include <fstream>
#include <deque>
#include <stack>
#include <vector>
#include <string>

const std::string inputFilePath = "day5.txt";
const int CRATE_CHAR_LENGTH = 3;

void moveCrateBetweenStacks(std::deque<char> &from, std::deque<char> &to){
    char crate = from.front();
    from.pop_front();
    to.push_front(crate);
}

void moveMultipleCratesBetweenStacks(std::deque<char> &from, std::deque<char> &to, int crateAmmount){
    std::stack<char> temporaryStack;
    for(int i=0; i< crateAmmount; i++){
        char crate = from.front();
        from.pop_front();
        temporaryStack.push(crate);
    }
    for(int i=0; i< crateAmmount; i++){
        const char crate = temporaryStack.top();
        temporaryStack.pop();
        to.push_front(crate);
    }
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    // Create stacks
    std::vector<std::deque<char>> stacks;
    std::getline(ifs, line);
    const int numberOfStacks = ((line.length() + 1) / (CRATE_CHAR_LENGTH + 1)); // +1 to account for ' ' between crates
    for(int i = 0; i <numberOfStacks; i++){
        std::deque<char> s; // We use deques instead of stacks so we can construct them while reading the file top down
        stacks.push_back(s);
    }

    // Parse diagram
    ifs.seekg (0, ifs.beg); // Reset to beggining
    while(std::getline(ifs, line) && line != ""){
        for(int i=0; i < line.length(); i++){
            if (line.at(i) == '['){
                stacks[i/4].push_back(line.at(i+1)); // +1 to go from '[' to char within brackets '[CHAR]'
            }
        }
    }

    std::vector<std::deque<char>> copyOfStacks = stacks; // To have separate copy for Ex2

    // Parse instructions
    while(std::getline(ifs, line)){
        const int crateAmmount = std::stoi(line.substr(line.find("move") + 5, line.find("from") - line.find("move") - 6));
        const int stackFrom = std::stoi(line.substr(line.find("from") + 5, line.find("to") - line.find("from") - 6));
        const int stackTo = std::stoi(line.substr(line.find("to") + 3));

        // Ex1:
        for(int i=0; i < crateAmmount;i++){
            moveCrateBetweenStacks(stacks[stackFrom-1], stacks[stackTo - 1]);
        } 

        // Ex2:
        moveMultipleCratesBetweenStacks(copyOfStacks[stackFrom-1], copyOfStacks[stackTo - 1], crateAmmount);
        
    }

    // Construct output strings from poping each stack
    std::string topCrates = "";
    for(int i=0; i < stacks.size();i++){
        topCrates.push_back(stacks[i].front());
    }

    std::string topCratesEx2 = "";
    for(int i=0; i < copyOfStacks.size();i++){
        topCratesEx2.push_back(copyOfStacks[i].front());
    }

    std::cout << "--Ex1 Output: " << topCrates << std::endl;
    std::cout << "--Ex2 Output: " << topCratesEx2 << std::endl;

    return 0;
}